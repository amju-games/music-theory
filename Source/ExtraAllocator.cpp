#include <AmjuAssert.h>
#include <AmjuRand.h>
#include "ExtraAllocator.h"
#include "NoteRun.h"

namespace Amju
{
// Get random value for points mutiplier reward
int GetRandomPointsMultiplier()
{
  // Let's say 80% chance of x2, 20% chance of x5.
  // As these accumulate we don't want to go crazy.
  return (Rnd(0.f, 1.f) > .8f ? 5 : 2); 
}

// Get random value for health boost reward
int GetRandomHealthBoost()
{
  // 60% chance of +2, 30% chance of +5, 10% chance of +10.
  const float r = Rnd(0.f, 1.f);
  return (r < .6f ? 2 : (r < .9 ? 5 : 10));
}

std::vector<int> CreatePointsForNoteRun(const NoteRun& run)
{
  const size_t numNotes = run.m_ids.size();
  std::vector<int> res;
  res.reserve(numNotes);
  // Points double; final value is sum of all previous points.
  int points = 100;
  int total = 0;
  for (size_t i = 0; i < numNotes - 1; ++i)
  {
    res.push_back(points);
    total += points;
    points *= 2;
  }
  res.push_back(total);
  Assert(res.size() == numNotes);
  return res;
}

// Return the number of note runs to which we should attempt to attach
//  extras, given the max number of runs in the song.
int GetNumExtraRuns(int maxNoteRuns)
{
  // TODO a configurable proportion - possibly within a range?
  return maxNoteRuns;
}

// Return the number of note-on events to which we should attempt to
//  attach extras, given the max number of note-on events in the song.
int GetNumRandomExtras(int maxNoteOnEvents)
{
  // TODO a configurable proportion - possibly within a range?
  return maxNoteOnEvents;
}
}

