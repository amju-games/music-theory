#include <array>
#include <map>
#include "TimeVal.h"

namespace MidiScore
{
int CalcTpqMultipleForTimeVal(int tpq, TimeVal t)
{
  // Calc multiple of tpq according to TimeVal, using int arith only.
  const std::array<int, 9> MULTS =
  {{
    0,  // for NONE - indication of error?
    tpq/8, tpq/4, tpq/2, tpq, tpq*2, tpq*4, tpq*8, tpq*16,
  }};
  int mult = MULTS[static_cast<int>(t)];
  return mult;
}

TimeVal GetTimeValFromString(const std::string& s)
{
  static const std::map<std::string, TimeVal> TVS =
  {
    { "qqq", TimeVal::QQQ },
    { "qq", TimeVal::SEMIQUAVER },
    { "q", TimeVal::QUAVER },
    { "c", TimeVal::CROTCHET },
    { "m", TimeVal::MINIM },
    { "sb", TimeVal::SEMIBREVE },
    { "sb2", TimeVal::SB2 },
    { "sb4", TimeVal::SB4 },
  };

  if (TVS.find(s) != TVS.end())
    return TVS.at(s);

  return TimeVal::NONE;
}

std::string TimeValString(TimeVal tv, int dots)
{
  static const std::array<std::string, 9> STRS =
  {{
    "NONE", "qqq", "qq", "q", "c", "m", "sb", "sb2", "sb4"
  }};

  std::string res = STRS[static_cast<int>(tv)];
  res += std::string(dots, '.');

  return res;
}
}

