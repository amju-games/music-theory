#pragma once

#include <vector>
#include "NoteEvent.h"

namespace Amju
{
// * Note Runs *
// A note run is a section of the song that forms part of a scale.
//  - Adjacent notes must be 1 or 2 semitones apart, (although we 
//    can set different allowed intervals to check for other features 
//    like arpeggios).
//  - No rests in between the notes
//  - All same duration? TODO - this can be optional.
//  - Run in same direction

// * NoteRun *
// Specifies one note run in a song (NoteEvents sequence).
struct NoteRun
{
  // Sequence of NoteEvent IDs
  std::vector<int> m_ids;
  // Asc/Descending
  bool m_isUp = false;
  // type: chromatic, major, minor, etc., or NONE if we can't identify.
  enum Type { RUN_NONE, RUN_CHROMATIC, RUN_MAJOR, RUN_MINOR, };
  Type m_type = RUN_NONE;
};
  
// Detect note runs in song.
std::vector<NoteRun> FindNoteRuns(
  const NoteEvents& allNoteEvents,
  int minPitchDiff, int maxPitchDiff);

// Debug print
void PrintRuns(const std::vector<NoteRun>& runs,
  const NoteEvents& allNoteEvents);

// Detect note runs in note events with only Note On and Rest On events.
// That simplifies the search a lot.
// Publicly exposed for testing.
std::vector<NoteRun> FindNoteRunsNoNoteOffEvents(
  const NoteEvents& noteEvents,
  int minPitchDiff, int maxPitchDiff);

// The algo is broken into forward-only passes over the note events.

// Pass 1
// Store note pitch diffs in vector the same length as allNoteEvents.
// Zero for all positions where diff doesn't make sense, e.g. 
//  for non-note events etc., and where the diff is too large/small to 
//  count as a run. So we pass in allowed diff range. 
std::vector<int> FindNoteDiffs(const NoteEvents& allNoteEvents,
  int minPitchDiff, int maxPitchDiff);

// Pass 2 
// Store vector of counts of contiguous runs of the same sign in the 
//  given vec of ints (the diffs from pass 1).
// Zeroes always give a count of zero.
// E.g. { 0, 1, 3, 5, 0, 6, -5, -4, -3, -2 } 
//  ->  { 0, 1, 2, 3, 0, 1,  1,  2,  3,  4 }
std::vector<int> CountSignRuns(const std::vector<int> diffs);
}

