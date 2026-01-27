// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include "Clef.h"

namespace MidiScore
{
std::string ClefString(Clef clef)
{
  if (clef == Clef::TREBLE) return "clef-t";
  return "clef-b";
}

Clef GuessClef(const Events& e)
{
  const auto [min, max] = std::minmax_element(e.begin(), e.end(),
    [](const Event& e1, const Event& e2) { return e1.m_pitch < e2.m_pitch; }
  );
  int min_pitch = min->m_pitch;
//  int max_pitch = max->m_pitch;
  if (min_pitch > 56) return Clef::TREBLE;
  return Clef::BASS;
}
}

