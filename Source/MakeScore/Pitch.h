// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>

struct Pitch
{
  int m_midi = 0;
  std::string m_step; // e.g. "c"
  int m_octave = 0;
  int m_alter = 0; // sharp (+) => 1, flat (-) => -1, etc

  void CalcMidi();
};

// Return true if token defines a pitch for use with subsequent tokens
// Pitches can be MIDI note values, or step/octave/alter format,
//  e.g. c4, d4-, e5++
bool IsDeferredPitch(const std::string& s);

// Return true if token defines a pitch, and should generate a note
//  immediately
bool IsImmediatePitch(const std::string& s);

// Return MIDI pitch for the given token, which should be a deferred
//  or immediate pitch value.
Pitch GetPitch(const std::string& s);
