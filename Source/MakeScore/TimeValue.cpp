// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <map>
#include "TimeValue.h"
#include "Utils.h"

static const std::map<std::string, float> TIME_VAL_STRS =
{
  { "sb", TIMEVAL_SEMIBREVE },
  { "m",  TIMEVAL_MINIM },
  { "c",  TIMEVAL_CROTCHET },
  { "q",  TIMEVAL_QUAVER },
  { "qq", TIMEVAL_SEMIQUAVER },
};

float GetTimeVal(std::string s)
{
  Remove(s, 'r'); // rests and notes are treated the same
  Remove(s, '*'); // in case glyph is hidden

  float dot = 1.f;
  if (Contains(s, '.'))
  {
    dot = 1.5f;
    Remove(s, '.');
  }

  auto it = TIME_VAL_STRS.find(s);
  if (it == TIME_VAL_STRS.end())
  {
    return -1;
  }

  return dot * it->second;
}

