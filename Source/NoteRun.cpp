#include <iostream>
#include <Sign.h>
#include "NoteRun.h"

namespace Amju
{
#ifdef NOTE_RUN_DEBUG
// Print int vec (diffs or sign runs)
static std::ostream& operator<<(std::ostream& os, const std::vector<int> vec)
{
  os << " { ";
  for (int i : vec) { os << i << " "; }
  return os << " } ";
}
#endif

// Debug print
void PrintRuns(const std::vector<NoteRun>& runs,
  const NoteEvents& events)
{
  std::cout << "* Note Runs *\n";
  int num = runs.size();
  for (int i = 0; i < num; i++)
  {
    const NoteRun& run = runs[i];
    std::cout << i << " / " << num << ": ";
    for (int id : run.m_ids)
    {
      std::cout << events[id].m_note << " ";
    }
    std::cout << (run.m_isUp ? "up" : "down")
      << "\n";
  }
}

// Min number of notes within the difference range to count as a note run.
// This is the lowest number that makes sense, as 1 means every note is a
//  run, and 2 means every pair of notes within an interval is a run.
static const int MIN_RUN_LENGTH = 3;

// Possibly add a note run to the given result vec.
// We only add if the run is long enough (min length could be a param).
// Having the min length check here slightly simplifies the algo code,
//  which is good because I don't like how complicated it is.
// Return true if we add the run to the result.
// Note on "allNoteEvents" param: this should only contain note-on and
//  rest-on events, particularly not note-off events. That's so we can
//  loop over events and not worry about jumping over note-off events.
static bool PossiblyAddRun(
  std::vector<NoteRun>& res, // result vector of note runs
  int runLength,  // number of notes in run
  int start, // index into note events: points to first note in run
  int sign, // +1 = ascending, -1 = descending
  const NoteEvents& allNoteEvents) // all events, in which the run lives.
{
#ifdef NOTE_RUN_DEBUG
std::cout << "Poss add: start: " << start << " len: " << runLength << "\n"; 
#endif

  // Save the run if > min length.
  // When we add it, identify the run type: chromatic, major, minor, etc.
  if (runLength >= MIN_RUN_LENGTH)
  {
    // Find the note on events between [start, end) and add to the run.
    // This is easy as we have stripped out the note-off events: all
    //  the events we want are contiguous.
    std::vector<int> ids;
    for (int i = start; i < start + runLength; ++i)
    {
      ids.push_back(allNoteEvents[i].m_id);
    }
    NoteRun nr { ids, sign > 0 };
//    nr.IndentifyType(allNoteEvents); TODO
    res.emplace_back(nr);
    return true;
  }
  return false;
}

// Count the length of runs of values which have the same sign.
std::vector<int> CountSignRuns(const std::vector<int> diffs)
{
  auto res(diffs); // result vec: same length as diffs
  int prevSign = 0; 
  res[0] = prevSign;

  // Loop over the diffs, checking if the sign has changed from the
  //  previous to the current element.
  for (int i = 1; i < diffs.size(); i++)
  {
    int s = Sign(diffs[i]);
    if (s == 0)
    {
      // Zeros are ignored
      res[i] = 0;
    }
    else if (s == prevSign) 
    { 
      // Same sign as previous: increment the count for this entry.
      res[i] = res[i - 1] + 1;  
    }
    else // s != prevSign
    { 
      // Sign has changed: reset the count to 1.
      // (Not 0: there is one element with sign s.)
      res[i] = 1; 
    }
    prevSign = s;
  }
  return res;
}

// Populate a vec with the difference in pitch between each note-on event.
std::vector<int> FindNoteDiffs(const NoteEvents& allNoteEvents,
  int minPitchDiff, int maxPitchDiff)
{
  const int numNoteEvents = static_cast<int>(allNoteEvents.size());

  // Create a vec of ints of the same size as allNoteEvents.
  std::vector<int> diffs;
  diffs.resize(numNoteEvents);
  diffs[0] = 0;

  // Populate diffs vec: each element is the difference between the
  //  current and previous pitch value.
  for (int i = 1; i < numNoteEvents; ++i)
  {
    const auto& prev = allNoteEvents[i - 1];
    const auto& ne = allNoteEvents[i];

    const bool isNoteOnEvent = (ne.m_type == NoteEventType::NOTE_ON);
    const bool isPrevNoteOnEvent = (prev.m_type == NoteEventType::NOTE_ON);

    diffs[i] = 0;
    // If this or the previous event are not a note-on event, skip.
    // Only diffs between two note-on events make sense.
    if (!isNoteOnEvent) continue;
    if (!isPrevNoteOnEvent) continue;

    const int pitchDiff = ne.m_note - prev.m_note; 
    const int absDiff = std::abs(pitchDiff);
    // To make things simpler in the next pass, check the abs diff and
    //  leave the result value as zero if the diff is too great to count
    //  as part of a note run.
    const bool isPitchInRange = 
      (absDiff >= minPitchDiff && absDiff <= maxPitchDiff);
    if (!isPitchInRange) continue;

    // Diff is in range, set the value.
    diffs[i] = pitchDiff;
  }
  return diffs;
}

// Detect scale-like runs of notes in a sequence of note-on and rest-on
//  events.
// It's simpler to not have to worry about note-off events: all members
//  of a note run will be contiguous.
std::vector<NoteRun> FindNoteRunsNoNoteOffEvents(
  const NoteEvents& allNoteEvents,
  int minPitchDiff, int maxPitchDiff)
{
#ifdef NOTE_RUN_DEBUG
std::cout << "** Starting search for note runs **\n";
#endif

  std::vector<NoteRun> res;
  int start = 0; // index of first note in run

  // Get pitch difference between each note-on pair.
  // If diff is out of range we set it to zero. So this is the place
  //  where min and max pitch diff are actually used.
  auto diffs = FindNoteDiffs(allNoteEvents, minPitchDiff, maxPitchDiff);

#ifdef NOTE_RUN_DEBUG
std::cout << "  Diffs: " << diffs << "\n";
#endif

  // Count the run length of diffs with the same sign.
  auto runs = CountSignRuns(diffs);

#ifdef NOTE_RUN_DEBUG
std::cout << "  Runs:  " << runs << "\n";
#endif

  // Loop over the sign run lengths. When the run length drops down,
  //  we have reached the end of the previous run and consider adding it
  //  to the result.
  for (int i = 1; i < runs.size(); ++i)
  {
    // Run length drop?
    if (runs[i] == 0 || (runs[i] < runs[i - 1]))
    {
      // Possibly add the run that ended at the previous element.
      int runLength = runs[i - 1] + 1;
      PossiblyAddRun(res, runLength, start, Sign(diffs[i - 1]), allNoteEvents);

      // Reset index of first note in run
      start = i;
    }
    else if (runs[i] == runs[i - 1])
    {
      // Same value - so values must be 1, 1 or 0, 0; reset index for
      //  the next run.
      start = i - 1;
    }
    // If the current value > previous, that's ok, the run continues.
  }

  // The loop won't add the final run: possibly add it now.
#ifdef NOTE_RUN_DEBUG
std::cout << "Final add: ";
#endif

  PossiblyAddRun(res, runs.size() - start, start, Sign(diffs.back()), 
    allNoteEvents);

  return res;
}

std::vector<NoteRun> FindNoteRuns(
  const NoteEvents& cNoteEvents,
  int minPitchDiff, int maxPitchDiff)
{
  // Remove note off events, to make things simpler.
  // When we search noteEvents, we only care about note and rest ON events.

  // Create copy of const input
  auto noteEvents(cNoteEvents);

  // Strip out everything except note on and rest on events
  noteEvents.erase(
    std::remove_if(noteEvents.begin(), noteEvents.end(),
    [](const NoteEvent& ne) 
    {   
      return !ne.IsNoteOnEvent() && !ne.IsRestOnEvent();
    }), 
    noteEvents.end());

  // Find runs in the simplified input. 
  return FindNoteRunsNoNoteOffEvents(noteEvents, minPitchDiff, maxPitchDiff);
}
}

