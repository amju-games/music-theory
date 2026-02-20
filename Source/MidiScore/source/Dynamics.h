#pragma once

#include <string>

namespace MidiScore
{
struct Dynamics
{
  // Set the last dynamics string output; this is to prevent
  //  duplicates, so we only output a new dynamic string when there's
  //  a change.
  static void SetLastDynamicsString(const std::string& s = "");

  // For final output, combining any dynamics markings and hairpins.
  // Can be empty if there is no need to give dynamics for the owning event.
  std::string ToString() const;

  void SetVelocity(int velocity) { m_velocity = velocity; }

  // Set dynamic mark ("p", "f") from velocity: 
  //  called if we want to attach a dynamics mark to the owning Event.
  std::string SetMark(); 

  int m_velocity = 0; // MIDI note on velocity
  std::string m_string; // "mp", "mf", etc, only for notes we want to give a dynamics marking to.
  // Hairpins
};

// Convert MIDI note velocity into a string.
std::string VelocityToString(int velocity);
}

