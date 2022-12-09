// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>

enum class Clef
{
  CLEF_TREBLE,
  CLEF_BASS,
  CLEF_ALTO,
  CLEF_TENOR
};

// Return true if input token defines a clef.
bool IsClef(const std::string& s);

// Return clef type from input token.
Clef GetClef(const std::string& s);

// Generate output string for the given clef type, on the stave numbered from 0
//  and going down the page. So for a single stave, staveNumber is zero. For the
//  lower stave of a double-stave score (e.g. piano music), staveNumber is 1, etc.
std::string GetClefOutputString(Clef clef, int staveNumber, float x, float y, float scale);
