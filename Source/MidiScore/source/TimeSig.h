// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#pragma once

#include <string>
#include "Event.h"

namespace MidiScore
{
enum class TimeSig
{
  TS_NONE,
  TS_2_4,
  TS_3_4,
  TS_4_4,
  TS_6_4,
  TS_3_8,
  TS_6_8,
  TS_9_8,
  TS_12_8,
};

// Get TimeSig value from string, used to parse time sig in 
//  command line input, which overrides guessing.
TimeSig GetTimeSigFromString(const std::string& s);

// Convert TimeSig to string for juliet/makescore
std::string TimeSigString(TimeSig ts);

// I.e. 4 in 4/4; 3 in 3/4.
// For compound times: 1.5 for 3/8, 3 for 6/8, 4.5 for 9/8, 6 for 12/8.
// For the use case for this function, placing bar lines, this is
//  the best option.
float BeatsInBar(TimeSig ts);

// From the given events and tpq, guess the time sig.
TimeSig GuessTimeSig(int tpq, const Events& events);
}

