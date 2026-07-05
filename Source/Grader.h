#pragma once

#include <optional>
#include "MusicEvent.h"
#include "NoteEvent.h"

namespace Amju
{
// Result of comparing player input event with the event in the song
//  which is the best match.
struct Grade
{
  enum Type { UNGRADED, GOOD_NOTE, NO_ATTEMPT, BAD_NOTE, TOO_QUICK, TOO_SLOW };
  Type m_type = UNGRADED;

  // Grade score: range is 0..1, 1 being perfect.
  float m_score = 0;
  std::string m_feedback;

  Grade(Type t, float score);
};

// * Grader *
// Hero Mode: Grades player input against expected next event
class Grader
{
public:
  // Public for testing: return range of note events to consider
  static std::pair<NoteEvents::const_iterator, NoteEvents::const_iterator>
    GetRangeToConsider(
    const MusicKbEvent& e, 
    const NoteEvents& noteEvents, 
    float animTime, 
    float songLength);

  // Public for testing: decide which event in range corresponds to player input
  static std::optional<NoteEvents::const_iterator> FindBestMatch(
    const NoteEvents::const_iterator beforeIt,
    const NoteEvents::const_iterator afterIt,
    const MusicKbEvent& e, float animTime, float songLength);

  // Return event that is closest match to player input
  static std::optional<NoteEvents::const_iterator> GetClosestMatchingEvent(
    const MusicKbEvent& e, 
    const NoteEvents& noteEvents, 
    float animTime, 
    float songLength);

  // Decide grade, based on 
  //  1. is note correct?
  //  2. grade  difference between normalised note event time and 
  //     normalised time now.
  // Pass in song length to convert normalised times to actual times.
  // Pass in max time (seconds), beyond which the event is too quick 
  //  or too slow.
  // Value in returned grade ranges from 0 (worst) to 1 (perfect).
  static Grade FinalGrade(
    const MusicKbEvent& e, 
    const NoteEvent& ne, 
    float animTimeNow, 
    float songLength,
    float maxTimeErrorSeconds);
};
}

