#pragma once

#include <vector>

namespace Amju
{
// All decisions about Extra allocations are grouped here.
// E.g.: points distribution, number of runs, random extras, etc.
// Then we can make all these decision points visible, testable
//  and configurable.

// Get random value for points mutiplier reward
int GetRandomPointsMultiplier();

// Get random value for health boost reward
int GetRandomHealthBoost();

// Set up a vec of points to award on each Extra along a run of notes.
// The final reward is the sum of the preceding points, if the player
//  hits all the notes in the run.
struct NoteRun;
std::vector<int> CreatePointsForNoteRun(const NoteRun& run);

// Return the number of note runs to which we should attempt to attach
//  extras, given the max number of runs in the song.
int GetNumExtraRuns(int maxNoteRuns);

// Return the number of note-on events to which we should attempt to
//  attach extras, given the max number of note-on events in the song.
int GetNumRandomExtras(int maxNoteOnEvents);
}

