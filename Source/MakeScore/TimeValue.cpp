// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <map>
#include "Consts.h"
#include "TimeValue.h"
#include "Utils.h"

static const std::map<std::string, float> TIME_VAL_STRS =
{
  { INPUT_TOKEN_SEMIBREVE,  TIMEVAL_SEMIBREVE },
  { INPUT_TOKEN_MINIM,      TIMEVAL_MINIM },
  { INPUT_TOKEN_CROTCHET,   TIMEVAL_CROTCHET },
  { INPUT_TOKEN_QUAVER,     TIMEVAL_QUAVER },
  { INPUT_TOKEN_SEMIQUAVER, TIMEVAL_SEMIQUAVER },
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

bool IsImmediateTimeVal(const std::string& cs)
{
  std::string s(cs);
  // Strip dot (.), rest (r), hide (*) qualifiers
  Remove(s, '.');
  Remove(s, 'r');
  Remove(s, '*');
  // TODO Any other qualifiers? NB Use consts!

  // Now we can check the remainder of the string to see if it's a recognised
  //  note type.
  return TIME_VAL_STRS.find(s) != TIME_VAL_STRS.end();
}

bool IsDeferredTimeVal(const std::string& s)
{
  return s.size() > 2 && s[0] == '<' && s.back() == '>' &&
    IsImmediateTimeVal(s.substr(1, s.size() - 2));
}
