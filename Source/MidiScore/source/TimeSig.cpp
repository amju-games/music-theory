// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include <array>
#include <map>
#include "TimeSig.h"

namespace MidiScore
{
int Numerator(TimeSig ts)
{
  static const std::array<int, 9> NUMS = 
  {{
    1, // TS_NONE -- not sure what would happen here! 
    2, //"2/4",
    3, //"3/4",
    4, //"4/4",
    6, //"6/4",
    3, //"3/8",
    6, //"6/8",
    9, //"9/8",
    12, //"12/8",
  }};

  return NUMS[static_cast<int>(ts)]; 
}

int Denominator(TimeSig ts)
{
  static const std::array<int, 9> DENOMS = 
  {{
    1, // TS_NONE
    4, //"2/4",
    4, //"3/4",
    4, //"4/4",
    4, //"6/4",
    8, //"3/8",
    8, //"6/8",
    8, //"9/8",
    8, //"12/8",
  }};

  return DENOMS[static_cast<int>(ts)]; 
}

bool IsCompoundTimeSig(TimeSig ts)
{
  return (
    ts == TimeSig::TS_6_8 ||
    ts == TimeSig::TS_9_8 ||
    ts == TimeSig::TS_12_8
  );
}

TimeSig GetTimeSigFromString(const std::string& s)
{
  static const std::map<std::string, TimeSig> TS_MAP = 
  {
    { "2/4", TimeSig::TS_2_4 },
    { "3/4", TimeSig::TS_3_4 },
    { "4/4", TimeSig::TS_4_4 },
    { "6/4", TimeSig::TS_6_4 },
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
  static const std::array<std::string, 9> STRS = 
  {{
    "", // TS_NONE
    "2/4",
    "3/4",
    "4/4",
    "6/4",
    "3/8",
    "6/8",
    "9/8",
    "12/8",
  }};

  return STRS[static_cast<int>(ts)]; 
}

float BeatsInBar(TimeSig ts)
{
  static const std::array<float, 9> BEATS = 
  {{
    1.f, // TS_NONE
    2.f,
    3.f,
    4.f,
    6.f,
    1.5f,
    3.f,
    4.5f,
    6.f,
  }};

  return BEATS[static_cast<int>(ts)];
}

TimeSig GuessTimeSig(int tpq, const Events& events)
{
  // Oh no, TODO!!!!!
  return TimeSig::TS_4_4;
}
}

