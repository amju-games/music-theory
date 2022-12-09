// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <map>
#include "Clef.h"
#include "Utils.h"

namespace
{
  std::map<std::string, Clef> CLEF_NAME
  {
    { "clef-t",     Clef::CLEF_TREBLE },
    { "clef-b",     Clef::CLEF_BASS },
    { "clef-alto",  Clef::CLEF_ALTO },
    { "clef-tenor", Clef::CLEF_TENOR }
  };
}

bool IsClef(const std::string& s)
{
  return Contains(s, "clef");
}

Clef GetClef(const std::string& s)
{
  return CLEF_NAME[s];
}

std::string GetClefOutputString(Clef clef, int staveNumber, float x, float y, float scale)
{
  std::string CLEF_OUTPUT_STR[4] = 
  {
    "treble-clef",
    "bass-clef",
    "alto-clef",
    "tenor-clef"
  };
  int c = static_cast<int>(clef);
  return CLEF_OUTPUT_STR[c] + 
    ", " + Str(x) + ", " + Str(y) + 
    ", " + Str(scale) + ", " + Str(scale);
}
