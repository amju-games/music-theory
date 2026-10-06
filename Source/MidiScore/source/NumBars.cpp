#include <MidiFile.h>
#include "NumBars.h"

namespace MidiScore
{
int TicksPerBar(smf::MidiFile& midifile, TimeSig ts)
{
  int tpq = midifile.getTPQ(); // Ticks Per Quarter note
  int ticksPerBar = static_cast<int>(
    tpq * Numerator(ts) * (4.0 / Denominator(ts)));

  return ticksPerBar;
}

int NumBars(smf::MidiFile& midifile, int track, TimeSig ts)
{
    smf::MidiEventList& eventList = midifile[track];
    if (eventList.size() == 0) {
        return {};
    }

    // 1. Calculate length of one bar in MIDI ticks
    int tpq = midifile.getTPQ(); // Ticks Per Quarter note
    int ticksPerBar = static_cast<int>(
        tpq * Numerator(ts) * (4.0 / Denominator(ts)));

    if (ticksPerBar <= 0) return 0;

    // 2. Find total duration to determine the number of bars
    int maxTick = 0;
    for (int i = 0; i < eventList.size(); ++i) {
        if (eventList[i].isNoteOn() || eventList[i].isNoteOff()) {
            maxTick = std::max(maxTick, eventList[i].tick);
        }
    }

    if (maxTick == 0) return 0;

    int totalBars = (maxTick + ticksPerBar - 1) / ticksPerBar;
    return totalBars;
}
}

