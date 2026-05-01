#ifdef GAME_ROUND_DEBUG
#include <iostream>
#endif

#include <algorithm>
#include <cmath> // round()
#include <map>
#include "Grader.h"

//#define GRADE_DEBUG

namespace Amju
{
static std::string GetFeedbackFromScore(float score)
{
  static const std::map<float, std::string> STRS = 
  {
    { .0f, "@@@Stinker!" },
    { .1f, "@@@Woeful!" },
    { .2f, "@@@Preposterous!" },
    { .3f, "@@@Pitiful!" },
    { .4f, "@@@Not bad!" },
    { .5f, "@@@Fanciful!" },
    { .6f, "@@@Smooth!" },
    { .7f, "@@@Good!" },
    { .8f, "@@@Delightful!!" },
    { .9f, "@@@Great!" },
    { .95f, "@@@Wonderful!" },
  };
  auto it = STRS.lower_bound(score);
  if (it == STRS.end())
  {
    return "@@@Perfect!";
  }
  return it->second;
}

Grade::Grade(Type t, float score) : 
  m_type(t), m_score(score) 
{
  if (t == Grade::TOO_QUICK)
  {
    m_feedback = "@@@Too Quick!";
  }
  else if (t == Grade::TOO_SLOW)
  {
    m_feedback = "@@@Too Slow!";
  }
  else if (t == Grade::NO_ATTEMPT)
  {
    m_feedback = "MISSED!";
  }
  else if (t == Grade::BAD_NOTE)
  {
    m_feedback = "BUM NOTE!";
  }
  else if (t == Grade::GOOD_NOTE)
  {
    m_feedback = GetFeedbackFromScore(score);
  }
  else
  {
    Assert(0);
  }
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

#ifdef GRADE_DEBUG
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

bool EventTypesMatch(const NoteEvent& ne, const MusicKbEvent& e)
{
  if (e.m_on && ne.IsNoteOnEvent())
  {
    return true;
  }
  
  if (!e.m_on && ne.IsNoteOffEvent())
  {
    return true;
  }

  return false;
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

    // Tricky logic alert.
    // We're looking for the best match in the score events for the event the 
    //  player has initiated. So the type should match, the time should be an
    //  improvement on the best match so far, and if it's a note off event,
    //  check the pitches match. If it's a note on event, the pitch could be 
    //  wrong, (but then so could the note off event? I'm not sure about this.)

    if (   EventTypesMatch(*it, e)  // event types match?
        && diff < bestDiff // closer time?
        && (e.m_on || IsPlayerPitchCorrect(it->m_note, e.m_note)))
          // if note off, do pitches match? (We are looking for the matching event here.)
          // if note is on, the match is the closest note on event.
    {
      bestDiff = diff;
      bestIt = it;
      found = true;
    }
  }

#ifdef GRADE_DEBUG
  if (found)
  {
    std::cout << "Best match:   Time: " << bestIt->m_time
      << (bestIt->m_onNotOff ? " ON " : " OFF ")
      << " Pitch: " << bestIt->m_note
      << " Time diff: " 
      << (animTime - bestIt->m_time) << " norm / "
      << songLength * (animTime - bestIt->m_time) << " secs"
      << "\n";
  }
  else
  {
    std::cout << "No best match?!?!\n";
  }
#endif

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
#ifdef GRADER_DEBUG
    // There are no note events! Hmm, not sure what this would mean.
std::cout << "No note events, so don't know what to do!\n";
#endif
    return std::nullopt;
  }

  const auto [beforeIt, afterIt] = GetRangeToConsider(e, noteEvents, animTime, songLength);

  return FindBestMatch(beforeIt, afterIt, e, animTime, songLength);
}

Grade Grader::FinalGrade(
  const MusicKbEvent& e, const NoteEvent& ne, float animTime, float songLength,
  float maxErrorSecs)
{
  if (!IsPlayerPitchCorrect(e.m_note, ne.m_note))
  {
    return Grade(Grade::BAD_NOTE, 0);
  }

  float timeDiff = animTime - ne.m_time;
  float timeDiffSecs = timeDiff * songLength;

#ifdef GRADE_DEBUG
std::cout << "Time diff: " << timeDiffSecs << "  seconds ";
#endif

  if (std::abs(timeDiffSecs) > maxErrorSecs)
  {
    if (timeDiffSecs < 0)
    {
#ifdef GRADE_DEBUG
std::cout << "Too quick, no points!\n";
#endif
      return Grade(Grade::TOO_QUICK, 0);
    }
#ifdef GRADE_DEBUG
std::cout << "Too slow, no points!\n";
#endif
    return Grade(Grade::TOO_SLOW, 0);
  }
  else
  {
    // We return a score from 0..1, where 1 is perfect.
    float grade = (1.f - std::abs(timeDiffSecs) / maxErrorSecs);
#ifdef GRADE_DEBUG
std::cout << " -- Grade: " << std::round(grade * 100.f) << "%\n";
#endif
    return Grade(Grade::GOOD_NOTE, grade);
  }
}
}

