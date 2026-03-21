#include <algorithm>
#include <iostream>
#include "FindSongSections.h"

namespace Amju
{
std::ostream& operator<<(std::ostream& os, const Section& s)
{
  return os << "[" << s.first << ", " << s.second << ")";
}

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
  if (events.empty() || s.second <= s.first || s.first >= static_cast<int>(events.size())) 
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

void AddGameRoundSections(Sections& sections, const std::vector<int>& bars)
{
}
}
