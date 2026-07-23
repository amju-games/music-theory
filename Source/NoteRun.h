#pragma once

#include <vector>
#include "NoteEvent.h"

namespace Amju
{
struct NoteRun
{
  // Range of indices into note events for the song.
  // [start, finish) -- int indices safer than iterators.
  int m_start = 0;
  int m_finish = 0;
  // up/down
  bool m_isUp = false;
  // type: chromatic, major, minor, etc.
  enum Type { RUN_NONE, RUN_CHROMATIC, RUN_MAJOR, RUN_MINOR, };
  Type m_type = RUN_NONE;
};
  
// Detect note runs in song.
std::vector<NoteRun> FindNoteRuns(const NoteEvents& allNoteEvents);

// Detect note runs in note events with only Note On and Rest On events.
// That simplifies the search a lot.
std::vector<NoteRun> FindNoteRunsNoNoteOffEvents(const NoteEvents& noteEvents);

// Store note pitch diffs in vector the same length as allNoteEvents.
// Zero for all positions where diff doesn't make sense, e.g. 
//  for non-note events etc., and where the diff is too large to 
//  count as a run.
std::vector<int> FindNoteDiffs(const NoteEvents& allNoteEvents);

// Vector of counts of contiguous runs of the same sign in the 
//  given vec of ints (which are diffs).
// Zeroes always give a count of zero.
// E.g. { 0, 1, 3, 5, 0, 6, -5, -4, -3, -2 } 
//  ->  { 0, 1, 2, 3, 0, 1,  1,  2,  3,  4 }
std::vector<int> CountSignRuns(const std::vector<int> diffs);
}

