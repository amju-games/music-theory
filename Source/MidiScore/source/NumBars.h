#pragma once

#include "TimeSig.h"

namespace MidiScore
{
// Utility functions for getting number of bars etc

int TicksPerBar(smf::MidiFile& midifile, TimeSig ts);

int NumBars(smf::MidiFile& midifile, int track, TimeSig ts);
}

