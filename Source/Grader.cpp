#include <algorithm>
#include <cmath> // round()
#include "Grader.h"

namespace Amju
{
static std::string GetFeedbackFromScore(float score)
{
  return "Fanciful!";
}

Grade::Grade(Type t, float score) : 
  m_type(t), m_score(score), m_feedback(GetFeedbackFromScore(score))
{
}

std::pair<NoteEvents::const_iterator, NoteEvents::const_iterator> 
  Grader::GetRangeToConsider(
  const MusicKbEvent& e, const NoteEvents& noteEvents, float animTime, float songLength)
{
  // Find note events before and after animTime. 
  // Creating a dummy NoteEvent to use as the search value for equal_range.
  //  (We want to search on animTime, which is not the same type
  //  as what's in the container. This is as good/bad as the alternatives :))
  NoteEvent searchVal;
  searchVal.m_time = animTime;
  // Get iterators to elements with times >= and > animTime
  auto [beforeIt, afterIt] = std::equal_range(
    noteEvents.begin(), noteEvents.end(), searchVal);

  // Expand range of events to include those preceding and following.
  if (beforeIt != noteEvents.begin())
  {
    // Drop down to previous element, so we can get the time of the last
    // event before animTime. 
    beforeIt--;
    // Now get all the events with the same time.
    searchVal.m_time = beforeIt->m_time;
    beforeIt = std::lower_bound(noteEvents.begin(), beforeIt, searchVal);
  }
  // Same idea, for the note events immediately after:
  if (afterIt != noteEvents.end())
  {
    afterIt++;
    if (afterIt != noteEvents.end())
    {
      // Include all events that are the next time up from animTime.
      searchVal.m_time = afterIt->m_time;
      afterIt = std::upper_bound(afterIt, noteEvents.end(), searchVal);
    }
  }

#ifdef CLOSEST_EVENT_DEBUG
std::cout << "KEY DOWN! Note: " << e.m_note << " Time now: " << animTime << ", note events considered:\n";
for (auto it = beforeIt; it != afterIt; ++it)
{
  std::cout << " Time: " << it->m_time
    << (it->m_onNotOff ? " ON " : " OFF ")
    << " Pitch: " << it->m_note
    << " Time diff: " << songLength * std::abs(animTime - it->m_time) << "s"
    << "\n";
}
#endif

  return std::make_pair(beforeIt, afterIt);
}

std::optional<NoteEvents::const_iterator> Grader::FindBestMatch(
  const NoteEvents::const_iterator beforeIt,
  const NoteEvents::const_iterator afterIt,
  const MusicKbEvent& e, float animTime, float songLength)
{
  // Find the most closely matching event: the same type, with the shortest
  //  time difference.
  // If this is a note off event, we're looking for the event with note off,
  //  same pitch as the player event, and closest time.
  bool found = false;
  float bestDiff = 1.f;
  auto bestIt = beforeIt;
  for (auto it = beforeIt; it != afterIt; ++it)
  {
    float diff = std::abs(animTime - it->m_time);

    if (   it->m_onNotOff == e.m_on  // event types match?
        && diff < bestDiff // closer time?
        && (e.m_on || it->m_note == e.m_note)) // if note off, pitches match?
    {
      bestDiff = diff;
      bestIt = it;
      found = true;
    }
  }

//#ifdef CLOSEST_EVENT_DEBUG
std::cout << "Best match:\n";
  std::cout << " Time: " << bestIt->m_time
    << (bestIt->m_onNotOff ? " ON " : " OFF ")
    << " Pitch: " << bestIt->m_note
    << " Time diff: " << songLength * std::abs(animTime - bestIt->m_time) << "s"
    << "\n";
//#endif

  if (!found)
  {
    return std::nullopt;
  }

  return bestIt;
}

std::optional<NoteEvents::const_iterator> Grader::GetClosestMatchingEvent(
  const MusicKbEvent& e, const NoteEvents& noteEvents, float animTime, float songLength)
{
  if (noteEvents.empty())
  {
    // There are no note events! Hmm, not sure what this would mean.
std::cout << "No note events, so don't know what to do!\n";
    return std::nullopt;
  }

  const auto [beforeIt, afterIt] = GetRangeToConsider(e, noteEvents, animTime, songLength);

  return FindBestMatch(beforeIt, afterIt, e, animTime, songLength);
}

Grade Grader::GradeTime(const NoteEvent& ne, float animTime, float songLength)
{
  float timeDiff = animTime - ne.m_time;
  float timeDiffSecs = timeDiff * songLength;

  const float PERFECT = 0.05f; // TODO TEMP TEST make these configurable
std::cout <<
  (std::abs(timeDiffSecs) < PERFECT ? " - *PERFECT TIMING*!" :
  (timeDiff < 0 ? " -- Too quick! " : " -- Too slow! "))
  << timeDiffSecs << "seconds ";

  const float MAX_ERROR = 0.25f; // TODO TEMP TEST
  if (std::abs(timeDiffSecs) > MAX_ERROR)
  {
std::cout << "NO POINTS!\n";
    return Grade(Grade::TOO_QUICK, 0);
  }
  else
  {
    // We could return a score from 0..1, or -1..1, where 0 is perfect,
    //  and the sign tells you whether we were early or late.
    float grade = (1.f - std::abs(timeDiffSecs) / MAX_ERROR);
std::cout << " -- Grade: " << std::round(grade * 100.f) << "%\n";
    return Grade(Grade::GOOD_NOTE, grade);
  }
}
}

