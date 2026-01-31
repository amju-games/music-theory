#pragma once

#include <string>

namespace MidiScore
{
struct Dynamics
{
  void SetVelocity(int velocity) { m_velocity = velocity; }

  // Use the previous note event dynamics and this one to set
  //  change in vel.
  void SetFromPrevEvent(const Dynamics& prevEvent);

  void SetString(); // Set string from velocity: called if we want to
    // attach a dynamics marking to the owning Event.

  int m_velocity = 0; // MIDI note on velocity
  int m_velChange = 0; // Change since previous note velocity
  std::string m_string; // "mp", "mf", etc, only for notes we want to give a dynamics marking to.
  // Hairpins
};

// Convert MIDI note velocity into a string.
std::string VelocityToString(int velocity);
}

