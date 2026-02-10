// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>

enum class Clef
{
  CLEF_NONE,
  CLEF_PERCUSSION,
  CLEF_TREBLE,
  CLEF_BASS,
  CLEF_ALTO,
  CLEF_TENOR,
  // Cue clefs: any clef defined after the start of a stave is a cue clef,
  //  which is then used as the main clef at the start of subsequent lines.
  CUE_CLEF_PERCUSSION, // ? 
  CUE_CLEF_TREBLE,
  CUE_CLEF_BASS,
  CUE_CLEF_ALTO,
  CUE_CLEF_TENOR,
  // Used internally to align bars vertically
  CLEF_BLANK,
  CUE_CLEF_BLANK,
};

// Return true if input token defines a clef.
bool IsClef(const std::string& s);

// Return clef type from input token.
Clef GetClef(const std::string& s);

// Generate output string for the given clef type.
std::string GetClefOutputString(Clef clef, float x, float y, float scale);

