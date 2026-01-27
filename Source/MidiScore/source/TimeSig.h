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

TimeSig GuessTimeSig(int tpq, const Events& events);
}

