#pragma once

#include <vector>
#include "TimeSig.h"

namespace smf
{
class MidiFile;
}

namespace MidiScore
{
// Gets the number of simultaneous voices in each bar, (using TimeSig
//  to get bar lengths), for the given track in the midifile.
std::vector<int> GetPolyphonyLevelPerBar(
  smf::MidiFile& midifile, // not const because we link note pairs :(
  int track,
  TimeSig ts);

// Splits trackIndex into multiple monophonic tracks. 
// Returns new track indices.
std::vector<int> SplitPolyTrack(
  smf::MidiFile& midifile,
  int trackIndex,
  TimeSig ts);
}

