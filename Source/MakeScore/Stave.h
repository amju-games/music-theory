// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>

enum class StaveType
{
  // No stave line
  STAVE_TYPE_NONE,

  // Single line, for rhythm notation with no pitch
  STAVE_TYPE_RHYTHM,

  // Single 5-line stave, e.g. for a flute part, say.
  STAVE_TYPE_SINGLE,

  // Double 5-line stave, e.g. for piano.
  STAVE_TYPE_DOUBLE,
};

// Look up StaveType from given string
StaveType GetStaveType(const std::string& s);

// Return true if this string defines a stave type
bool IsStaveType(const std::string& s);

// Get the output glyphs for the given stave type, 
//  given (x, y) position and scale.
std::string GetStaveString(StaveType st, float x, float y, float scaleX, float scaleY);


