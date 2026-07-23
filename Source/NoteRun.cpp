#include <iostream>
#include <Sign.h>
#include "NoteRun.h"

namespace Amju
{
static std::ostream& operator<<(std::ostream& os, const std::vector<int> vec)
{
  os << " { ";
  for (int i : vec) { os << i << " "; }
  return os << " } ";
}

static const int MIN_RUN_LENGTH = 3;
static const int MAX_DIFF = 3; // max semitone difference to count as a run
static const int MIN_DIFF = 1; // parameterise these for finding arpegs.

static bool PossiblyAddRun(
  std::vector<NoteRun>& res, int runLength, int start, int sign, 
  const NoteEvents& allNoteEvents)
{
std::cout << "Poss add: start: " << start << " len: " << runLength << "\n"; 
  // Save previous run if > min length.
  // When we add it, identify the run type: 
  //  none, chromatic, major, minor, etc.
  if (runLength >= MIN_RUN_LENGTH)
  {
    // Look up IDs, don't use indices. This is so we can remove
    //  note events to simplify the algo, but point to the correct
    //  events in the full sequence.
    const int startId = allNoteEvents[start].m_id;
    int endId = allNoteEvents.back().m_id + 1; // Or should this be inclusive?
    if (start + runLength < allNoteEvents.size())
    {
      endId = allNoteEvents[start + runLength].m_id;
    }
    NoteRun nr { startId, endId, sign > 0 };
//    nr.IndentifyType(allNoteEvents);
    res.emplace_back(nr);
    return true;
  }
  return false;
}

std::vector<int> CountSignRuns(const std::vector<int> diffs)
{
  auto res(diffs);
  int prevSign = 0; //Sign(diffs[0]);
  res[0] = prevSign;
  for (int i = 1; i < diffs.size(); i++)
  {
    int s = Sign(diffs[i]);
    res[i] = 0;
    if (s == prevSign && s != 0) { res[i] = res[i - 1] + 1; }
    else if (s != prevSign && s != 0) { res[i] = 1; }
    prevSign = s;
  }
  return res;
}

std::vector<int> FindNoteDiffs(const NoteEvents& allNoteEvents)
{
  const int numNoteEvents = static_cast<int>(allNoteEvents.size());

  // Create a vec of ints of the same size as allNoteEvents.
  // The ints represent run length at the corresponding note.
  std::vector<int> diffs;
  diffs.resize(numNoteEvents);
  diffs[0] = 0;

  // Populate diffs vec: first pass: populate with valid diffs, ignoring
  //  sign changes
  for (int i = 1; i < numNoteEvents; ++i)
  {
    const auto& prev = allNoteEvents[i - 1];
    const auto& ne = allNoteEvents[i];

    const bool isNoteOnEvent = (ne.m_type == NoteEventType::NOTE_ON);
    const bool isPrevNoteOnEvent = (prev.m_type == NoteEventType::NOTE_ON);

    diffs[i] = 0;
    if (!isNoteOnEvent) continue;
    if (!isPrevNoteOnEvent) continue;

    const int pitchDiff = ne.m_note - prev.m_note; 
    const int absDiff = std::abs(pitchDiff);
    const bool isPitchInRange = (absDiff >= MIN_DIFF && absDiff <= MAX_DIFF);
    if (!isPitchInRange) continue;

    diffs[i] = pitchDiff;
  }
  return diffs;
}

// Detect note diffs
// Length of run of notes: 
//  - Notes must be 1 or 2 semitones apart
//  - No rests in between
//  - all same duration? TODO
//  - Run in same direction
//  - Essentially, part of a scale ascending or descending.
std::vector<NoteRun> FindNoteRunsNoNoteOffEvents(const NoteEvents& allNoteEvents)
{
std::cout << "** Starting search for note runs **\n";

  std::vector<NoteRun> res;
  int start = 0; // index of first note in run

  auto diffs = FindNoteDiffs(allNoteEvents);
std::cout << "  Diffs: " << diffs << "\n";

  auto runs = CountSignRuns(diffs);
std::cout << "  Runs:  " << runs << "\n";

  for (int i = 1; i < runs.size(); ++i)
  {
    if (runs[i] == 0 || (runs[i] < runs[i - 1]))
    {
      int runLength = runs[i - 1] + 1;
      PossiblyAddRun(res, runLength, start, Sign(diffs[i - 1]), allNoteEvents);
      start = i;
    }
    else if (runs[i] == runs[i - 1])
    {
      start = i - 1;
    }
  }

std::cout << "Final add: ";
  PossiblyAddRun(res, runs.size() - start, start, Sign(diffs.back()), allNoteEvents);
  return res;
}

std::vector<NoteRun> FindNoteRuns(const NoteEvents& cNoteEvents)
{
  // Remove note off events, to make things simpler
  // When we search noteEvents, we only care about note and rest ON events.
  // TODO Find a better way to do this: we are copying the vec and erasing.
  auto noteEvents(cNoteEvents);
  // Strip out everything except note on and rest on events
  noteEvents.erase(
    std::remove_if(noteEvents.begin(), noteEvents.end(),
    [](const NoteEvent& ne) 
    {   
      return !ne.IsNoteOnEvent() && !ne.IsRestOnEvent();
    }), 
    noteEvents.end());

   auto runs = FindNoteRunsNoNoteOffEvents(noteEvents);

   // Convert indices into noteEvents into indices into cNoteEvents?
   // No, we get that automatically by using IDs instead of indices when
   //  we make NoteRuns
   return runs;
}
}

