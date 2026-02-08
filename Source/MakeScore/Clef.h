// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
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

// Generate output string for the given clef type.
std::string GetClefOutputString(Clef clef, float x, float y, float scale);

