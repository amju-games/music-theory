// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include <array>
#include <map>
#include "TimeSig.h"

namespace MidiScore
{
TimeSig GetTimeSigFromString(const std::string& s)
{
  static const std::map<std::string, TimeSig> TS_MAP = 
  {
    { "2/4", TimeSig::TS_2_4 },
    { "3/4", TimeSig::TS_3_4 },
    { "4/4", TimeSig::TS_4_4 },
    { "3/8", TimeSig::TS_3_8 },
    { "6/8", TimeSig::TS_6_8 },
    { "9/8", TimeSig::TS_9_8 },
    { "12/8",  TimeSig::TS_12_8 },
  };
  const auto it = TS_MAP.find(s);
  if (it == TS_MAP.end()) return TimeSig::TS_NONE;
  return it->second;
}

std::string TimeSigString(TimeSig ts) 
{
  static const std::array<std::string, 8> STRS = 
  {{
    "", // TS_NONE
    "2/4",
    "3/4",
    "4/4",
    "3/8",
    "6/8",
    "9/8",
    "12/8",
  }};

  return STRS[static_cast<int>(ts)]; 
}

float BeatsInBar(TimeSig ts)
{
  static const std::array<float, 8> BEATS = 
  {{
    1.f, // TS_NONE
    2.f,
    3.f,
    4.f,
    1.5f,
    3.f,
    4.5f,
    6.f,
  }};

  return BEATS[static_cast<int>(ts)];
}

TimeSig GuessTimeSig(int tpq, const Events& events)
{
  if (events.empty()) return TimeSig::TS_NONE;
  int totalDuration = events.back().m_end;
  if (totalDuration % (3 * tpq) == 0) return TimeSig::TS_3_4;
  return TimeSig::TS_4_4;
}
}

