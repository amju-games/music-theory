// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <map>
#include "TimeSig.h"
#include "Utils.h"

static const std::map<std::string, TimeSig> TIME_SIG_STRS =
{
  { "3/8", TimeSig::TIME_SIG_THREE_EIGHT },
  { "6/8", TimeSig::TIME_SIG_SIX_EIGHT },
  { "9/8", TimeSig::TIME_SIG_NINE_EIGHT },
  { "12/8", TimeSig::TIME_SIG_TWELVE_EIGHT },
  { "2/4", TimeSig::TIME_SIG_TWO_FOUR },
  { "3/4", TimeSig::TIME_SIG_THREE_FOUR },
  { "4/4", TimeSig::TIME_SIG_FOUR_FOUR },
  { "5/4", TimeSig::TIME_SIG_FIVE_FOUR },
  { "6/4", TimeSig::TIME_SIG_SIX_FOUR },
  { "common", TimeSig::TIME_SIG_COMMON },
  { "cut-common", TimeSig::TIME_SIG_CUT_COMMON }, // TODO s/b compound glyph
};

bool IsCompoundTime(TimeSig ts)
{
  return 
    ts == TimeSig::TIME_SIG_THREE_EIGHT ||
    ts == TimeSig::TIME_SIG_SIX_EIGHT ||
    ts == TimeSig::TIME_SIG_NINE_EIGHT ||
    ts == TimeSig::TIME_SIG_TWELVE_EIGHT;
}

Beats_CrotchetValue GetNumBeatsAndCrotchetValue(TimeSig ts)
{
  static const std::map<TimeSig, Beats_CrotchetValue> TIMES = 
  {
    { TimeSig::TIME_SIG_NONE, { 0, 1.f } },
    { TimeSig::TIME_SIG_TWO_FOUR, { 2, 1.f } },
    { TimeSig::TIME_SIG_THREE_FOUR, { 3, 1.f } },
    { TimeSig::TIME_SIG_FOUR_FOUR, { 4, 1.f } },
    { TimeSig::TIME_SIG_FIVE_FOUR, { 5, 1.f } },
    { TimeSig::TIME_SIG_SIX_FOUR, { 6, 1.f } },
    { TimeSig::TIME_SIG_COMMON, { 4, 1.f } },
    { TimeSig::TIME_SIG_CUT_COMMON, { 2, 2.f } },
    { TimeSig::TIME_SIG_THREE_EIGHT, { 1, .666667f } },
    { TimeSig::TIME_SIG_SIX_EIGHT, { 2, .666667f } },
    { TimeSig::TIME_SIG_NINE_EIGHT, { 3, .666667f } },
    { TimeSig::TIME_SIG_TWELVE_EIGHT, { 4, .666667f } },
  };
  auto it = TIMES.find(ts);
  if (it != TIMES.end())
  {
    return it->second;
  }
  // ?
  return {};
}

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
  // Reverse look up
  for (auto it = TIME_SIG_STRS.begin(); it != TIME_SIG_STRS.end(); ++it)
  {
    if (it->second == ts)
    {
      return it->first;
    }
  }
  return "";
}

