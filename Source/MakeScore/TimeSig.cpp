// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <map>
#include "TimeSig.h"
#include "Utils.h"

static const std::map<std::string, TimeSig> TIME_SIG_STRS =
{
  { "2/4", TimeSig::TIME_SIG_TWO_FOUR },
  { "3/4", TimeSig::TIME_SIG_THREE_FOUR },
  { "4/4", TimeSig::TIME_SIG_FOUR_FOUR },
  { "common", TimeSig::TIME_SIG_COMMON },
  { "cut-common", TimeSig::TIME_SIG_CUT_COMMON }, // TODO s/b compound glyph
};

bool IsTimeSig(std::string s)
{
  // can be replaced with a star for 'what's the time sig' questions
  Remove(s, '*');

  auto it = TIME_SIG_STRS.find(s);
  return it != TIME_SIG_STRS.end();
}

TimeSig GetTimeSig(std::string s)
{
  // can be replaced with a star for 'what's the time sig' questions
  Remove(s, '*');

  auto it = TIME_SIG_STRS.find(s);
  return it->second;
}

std::string GetStr(TimeSig ts)
{
  for (auto it = TIME_SIG_STRS.begin(); it != TIME_SIG_STRS.end(); ++it)
  {
    if (it->second == ts)
    {
      return it->first;
    }
  }
  return "";
}

