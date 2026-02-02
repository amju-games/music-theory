// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>

struct Pitch
{
  Pitch() = default;

  explicit Pitch(int midi) : m_midi(midi) {}

  Pitch(const std::string& step, int octave, int alter) : 
    m_midi(0), m_step(step), m_octave(octave), m_alter(alter) {}

  int m_midi = 0;
  std::string m_step; // e.g. "c"
  int m_octave = 0;
  int m_alter = 0; // sharp (+) => 1, flat (-) => -1, etc

  void CalcMidi();

  std::string ToString() const;
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

