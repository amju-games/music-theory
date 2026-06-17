// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#pragma once

#include <string>
#include <vector>
#include "Event.h"

namespace MidiScore
{
enum class TimeSig;

enum class Clef
{
  TREBLE,
  BASS,
  ALTO,
  TENOR
};

std::string ClefString(Clef clef);

struct ClefChange
{
  int tick_time;
  Clef clef;
};

using ClefChanges = std::vector<ClefChange>;

// Choose clef, with possible changes throughout the given events.
// Returns the best clef for the start of the piece.
Clef GuessClef(const Events& e, int tpq, TimeSig ts, 
  ClefChanges& allClefChanges, // result: all the changes
  bool justTrebleAndBass = true);
}

