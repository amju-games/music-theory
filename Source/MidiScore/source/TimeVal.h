#pragma once

#include <string>

namespace MidiScore
{
enum class TimeVal
{
  NONE,

  QQQ,
  SEMIQUAVER,
  QUAVER,
  CROTCHET,
  MINIM,
  SEMIBREVE,
  SB2,
  SB4
};

// * TimeValString *
// Return string for timeval, in juliet notation format, suitable for
//  makescore.
std::string TimeValString(TimeVal t, int dots = 0);

// Return TimeVal from string, for converting command-line params to
//  TimeVals.
TimeVal GetTimeValFromString(const std::string& s);

// * CalcTpqMultipleForTimeVal *
// Calc tpq multiplied according to the TimeVal. A crotchet time val means
//  a multiplier of 1; minim multiplies by 2, quaver multiplies by 0.5, etc.
// All the arithmetic uses ints, and event start times and durations are ints,
//  so there are no float precision issues to worry about.
//
// E.g.: CalcTpqMultipleForTimeVal(16, <minim>) => 32
int CalcTpqMultipleForTimeVal(int tpq, TimeVal t);
}

