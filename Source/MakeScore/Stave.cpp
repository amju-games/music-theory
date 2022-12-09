// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include "Consts.h"
#include "Stave.h"
#include "Utils.h"

std::string GetStaveString(StaveType st, float x, float y, float scaleX, float scaleY)
{
  std::string res;

  switch (st)
  {
  case StaveType::STAVE_TYPE_NONE:
    break;

  case StaveType::STAVE_TYPE_RHYTHM:
    res += "stave-line, " + Str(x) + ", " + Str(DEFAULT_HEIGHT + y) + ", " +
      Str(scaleX) + ", " + Str(scaleY);
    break;

  case StaveType::STAVE_TYPE_SINGLE:
    res += "stave, " + Str(x) + ", " + Str(y) + ", " + Str(scaleX) + ", " + Str(scaleY);
    break;

  case StaveType::STAVE_TYPE_DOUBLE:
    res += "stave, " + Str(x) + ", " + Str(y) + ", " + Str(scaleX) + ", " + Str(scaleY);
    res += ";";
    res += "stave, " + Str(x) + ", " + Str(DOUBLE_STAVE_DISTANCE + y) + ", " +
      Str(scaleX) + ", " + Str(scaleY);
  }

  return res;
}
