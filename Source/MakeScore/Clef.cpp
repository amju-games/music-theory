// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
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

std::string GetClefOutputString(Clef clef, float x, float y, float scale)
{
  std::map<Clef, std::string> CLEF_OUTPUT = 
  {
    { Clef::CLEF_TREBLE, "treble-clef" },
    { Clef::CLEF_BASS, "bass-clef" },
    { Clef::CLEF_ALTO, "alto-clef" },
    { Clef::CLEF_TENOR, "tenor-clef" },
  };
  auto it = CLEF_OUTPUT.find(clef);
  if (it == CLEF_OUTPUT.end())
    return "";

  return it->second + 
    ", " + Str(x) + ", " + Str(y) + 
    ", " + Str(scale) + ", " + Str(scale);
}

