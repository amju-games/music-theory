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

// Clef change is represented by an Event of type EventType::CLEF.
using ClefChange = Event;
Event MakeClefChange(Clef clef, int timeInTpqTicks);

using ClefChanges = std::vector<ClefChange>;

// Choose clef, with possible changes throughout the given events.
// Returns the best clef for the start of the piece, i.e. allClefChanges[0].m_clef.
Clef GuessClef(
  const Events& e, 
  int tpq, 
  int anacrusisTicks, 
  TimeSig ts, 
  ClefChanges& allClefChanges, // result: all the changes
  bool justTrebleAndBass = true, // if true, no alto or tenor clefs in output
  int barsPerChunk = 4, // internally we find the best clef per chunk of score.
  int threshold = 2 // number of chunks of continuous difference to trigger clef change
);

// Insert clef changes into events: do this after barlines.
void InsertClefs(Events& e, const ClefChanges& cc); 
}
 
