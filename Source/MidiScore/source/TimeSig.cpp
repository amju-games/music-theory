// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include "TimeSig.h"

namespace MidiScore
{
std::string TimeSigString(TimeSig ts) 
{
  if (ts == TimeSig::TS_4_4) return "4/4 ";
  if (ts == TimeSig::TS_3_4) return "3/4 ";
  return ""; 
}

int BeatsInBar(TimeSig ts)
{
  if (ts == TimeSig::TS_3_4) return 3;
  return 4;
}

TimeSig GuessTimeSig(int tpq, const Events& events)
{
  int totalDuration = events.back().m_end;
  if (totalDuration % (3 * tpq) == 0) return TimeSig::TS_3_4;
  return TimeSig::TS_4_4;
}
}

