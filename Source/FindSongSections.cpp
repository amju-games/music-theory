#include <algorithm>
#include <iostream>
#include "FindSongSections.h"

namespace Amju
{
Sections FindSongSections(const NoteEvents& events)
{
  // Sections are pairs of event IDs. The IDs are the same as the
  //  indices in the events vector, but if we are not iterating over
  //  the vector, we don't know the index, so we need this redundancy.

  // First, easy algo: Identify sections by rests. Don't create a section
  //  that is just rests. I am a bit worried that this won't work for
  //  rests interleaved with notes but not sure if that's a real problem.
  Sections res;
  int start = 0;
  for (const auto& ne : events)
  {
//std::cout << "Event " << ne.GetId() << ": " << ToString(ne.m_type) << "\n";
    if (ne.IsRestOnEvent())
    {
      Section s(start, ne.GetId());
      if (s.second > s.first) 
      { 
std::cout << "Found section: " << s << "\n";
        res.push_back(s);
        start = s.second;
      }
    }
    else if (ne.IsRestOffEvent())
    {
      start = ne.GetId() + 1;
    }
  }

  // Final section
  Section s(start, events.back().GetId() + 1);
  if (s.second > s.first)
  {
    res.push_back(s);
std::cout << "Final section: " << res.back() << "\n";
  }

  return res;
}

std::optional<std::pair<int, int>> FindMinMaxPitchInSection(
  const Section& s, const NoteEvents& events)
{
  if (   events.empty() 
      || s.second <= s.first 
      || s.first >= static_cast<int>(events.size())) 
  {
    return std::nullopt;
  }

  // Get the min and max notes in the range. s gives us indices into 
  //  events, so convert to iterators..
  auto first = events.begin() + s.first;
  auto last = events.begin() + s.second;

  const auto [minIt, maxIt] = std::minmax_element(first, last, 
    [](const NoteEvent& ne1, const NoteEvent& ne2) 
    { 
      return ne1.m_note < ne2.m_note; 
    });
  const int minNote = minIt->m_note;
  const int maxNote = maxIt->m_note;

  if (minNote == -1 || maxNote == -1)
  {
    return std::nullopt;
  }
  return std::make_pair(minNote, maxNote); 
}

void InsertNewSections(Sections& sections, const std::vector<int>& ids,
  const NoteEvents& events)
{
  auto newIds(ids);
  for (const auto& s : sections)
  {
    newIds.push_back(s.first);
    newIds.push_back(s.second);
  }
  std::sort(newIds.begin(), newIds.end());
  newIds.erase(std::unique(newIds.begin(), newIds.end()), newIds.end());
  
  Sections newSections;
  
  for (int i = 1; i < newIds.size(); ++i)
  {
    int begin = newIds[i - 1];
    int end = newIds[i];
    while (begin <= end && !events[begin].IsNoteEvent())
    {
      ++begin;
    }
    while (end >= begin && !events[end].IsNoteEvent())
    {
      --end;
    }

    if (end > begin)
    {
      newSections.emplace_back(Section(newIds[i - 1], newIds[i]));
    }
  }

  sections = newSections;
}

void AddGameRoundSections(Sections& sections, const std::vector<int>& bars,
  const NoteEvents& events, const BeatVec& beats)
{
  // Bars must be in sorted ascending order.
  auto barIt = bars.begin();
  if (barIt == bars.end()) return;

std::cout << "Finding more sections from list of bar numbers... \n";

  // We only want note events
  auto noteEvents(events);
  noteEvents.erase(
    std::remove_if(noteEvents.begin(), noteEvents.end(), 
     [=](const NoteEvent& ne) { return !ne.IsNoteEvent(); }),
    noteEvents.end());

#ifdef ADD_SECTIONS_DEBUG
  for (auto b : bars)
  {
std::cout << b << " ";
  }
std::cout << "\n";
#endif //  ADD_SECTIONS_DEBUG

  // These are result IDs of the events we are searching for: they are the final
  //  event in one of the bars specified.
  std::vector<int> eventIds;

  // As we find the beat for a note, we can reduce the remaining 
  //  search space, because beats are sorted.
  // This also serves as pointing to the beat for the previous event,
  //  which is super handy for finding the last event in a given bar.
  auto beatsBegin = beats.begin();

  // Linear search through all events. Look up the beat for each event
  //  (log(n)), where the search space n is reduced every iteration.
  for (const auto& e : noteEvents)
  {
    const auto barToFind = *barIt; 

#ifdef ADD_SECTIONS_DEBUG
std::cout << "Event: " << e.ToString() << ":\n";
std::cout << "Bar to find: " << barToFind << "\n";
#endif //  ADD_SECTIONS_DEBUG

    // We want to find the entry in `beats` for the time of event e.  
    // What this does is find the lowest beat with time >= e.m_time. 
    auto it = std::lower_bound(beatsBegin, beats.end(), e.m_time,
      [](const Beat& b, float time) { return b.m_time < time; });

    if (it == beats.end())
    {
      // We are done; maybe final event was end of a section
      //  but it's probably not worth bothering with, right?
std::cout << "No more beats, so we are at the end of the song, right?\n";
      break;
    }
    const int barOfEvent = it->m_bar;

    // Before we overwrite beatsBegin: it is the beat of the last event we
    //  found. If that beat has a lower bar, one that we are interested in,
    //  (i.e. in `bars`) then we have found the final event of that bar.
    const int barOfPrevEvent = beatsBegin->m_bar;

#ifdef ADD_SECTIONS_DEBUG
std::cout << "  Bar: " << barOfEvent 
  << ", bar of prev event: " << barOfPrevEvent << "\n"; 
#endif //  ADD_SECTIONS_DEBUG

    if (barOfEvent > barToFind) 
    {
      if (barOfPrevEvent == barToFind)
      {
#ifdef ADD_SECTIONS_DEBUG
std::cout << "  -> Found! FINAL event in the bar!\n";
std::cout << "  Event: " << e.m_id - 1 << " is the final event in bar "
  << barToFind 
  << ".\n";
#endif //  ADD_SECTIONS_DEBUG
        // Store result.
        // This is the end marker of a section, and is exclusive, so 
        //  we use e.m_id, not e.m_id - 1.
        eventIds.push_back(e.m_id);
      }
      else
      {
#ifdef ADD_SECTIONS_DEBUG
std::cout << "No events in bar " << barToFind << "???\n";
#endif //  ADD_SECTIONS_DEBUG
      }

      barIt++; // go on to the next bar/event to find
      if (barIt == bars.end())
      {
#ifdef ADD_SECTIONS_DEBUG
std::cout << "No more bars to find, so we are done.\n";
#endif //  ADD_SECTIONS_DEBUG
        break; // No more events to find
      }
    }
    beatsBegin = it; // next time the beat must be after this
  }

//#ifdef ADD_SECTIONS_DEBUG
std::cout << "Result event IDs: ";
  for (int id : eventIds)
  {
std::cout << id << " ";
  }
std::cout << "\n";
//#endif //  ADD_SECTIONS_DEBUG

  // Create new sections from the existing sections and our list of event IDs
  //  that mark the end of (possibly new) sections.
  InsertNewSections(sections, eventIds, events);
}
}
