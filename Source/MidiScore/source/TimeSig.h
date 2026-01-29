// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#pragma once

#include <string>
#include "Event.h"

namespace MidiScore
{
enum class TimeSig
{
  TS_4_4,
  TS_3_4,
};

std::string TimeSigString(TimeSig ts);

// I.e. 4 in 4/4; 3 in 3/4; * 2 in 6/8, right?? *, etc.
int BeatsInBar(TimeSig ts);

TimeSig GuessTimeSig(int tpq, const Events& events);
}

