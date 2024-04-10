// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>

enum class Accidental
{
  ACCIDENTAL_NONE,
  ACCIDENTAL_NATURAL,
  ACCIDENTAL_NATURAL_IN_KEY_SIG,
  ACCIDENTAL_SHARP,
  ACCIDENTAL_SHARP_IN_KEY_SIG, // e.g. F in G major key
  ACCIDENTAL_FLAT,
  ACCIDENTAL_FLAT_IN_KEY_SIG,
  ACCIDENTAL_DOUBLE_SHARP,
  ACCIDENTAL_DOUBLE_FLAT
};

bool IsAccidental(const std::string& s);

Accidental GetAccidental(const std::string& s);

// TODO clef, pitch, stave num etc
std::string GetOuptutAccidental(Accidental);

