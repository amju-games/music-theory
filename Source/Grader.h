#pragma once

#include <optional>
#include "MusicEvent.h"
#include "NoteEvent.h"

namespace Amju
{
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

  // Decide grade based on difference between note event time and 
  //  (normalised) time now.
  static void GradeTime(const NoteEvent& ne, float animTimeNow, float songLength);
};
}

