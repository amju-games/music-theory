// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#pragma once

#include <string>
#include "Event.h"

namespace MidiScore
{
enum class Clef
{
  TREBLE,
  BASS,
};

std::string ClefString(Clef clef);

Clef GuessClef(const Events& e);
}

