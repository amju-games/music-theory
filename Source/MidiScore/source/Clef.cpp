// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include <algorithm>
#include <iostream>
#include "Clef.h"

namespace MidiScore
{
std::string ClefString(Clef clef)
{
  if (clef == Clef::TREBLE) return "clef-t";
  return "clef-b";
}

Clef GuessClef(const Events& constevents)
{
  // Remove events with a pitch of zero, so they don't skew our guesswork.

  auto e(constevents);
  e.erase(
    std::remove_if(e.begin(), e.end(), 
      [](const Event& event) { return event.m_pitch == 0; }), 
    e.end());

  const auto [min, max] = std::minmax_element(e.begin(), e.end(),
    [](const Event& e1, const Event& e2) { return e1.m_pitch < e2.m_pitch; }
  );
  int min_pitch = min->m_pitch;
  int max_pitch = max->m_pitch;

  std::cout << "// Guessing clef... min pitch: " << min_pitch 
    << " max pitch: " << max_pitch 
    << "\n";

  if (min_pitch > 56) return Clef::TREBLE;
  return Clef::BASS;
}
}

