// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#pragma once

#include <optional>
#include <string>
#include "Event.h"

namespace smf
{
class MidiFile;
}

namespace MidiScore
{
// Output text info about the given midi file.
std::string InfoString(smf::MidiFile& mf);

// Output midi file as juliet shorthand notation, for reading 
//  by MakeScore.
// mf: the midi file.
// track: optional track number; if nullopt, all tracks are output.
// timeSig: optional time signature, overrides guessing.
// keySig: optional key signature, overrides guessing.
// quantResolution: optional, sets quantisation on and resolution level.
// debug: true for debug mode, verbose for debugging.
// bpm: optional bpm tempo.
std::string ToString(
  smf::MidiFile& mf,
  std::optional<int> track,
  std::string timeSig, 
  std::optional<int> keySig,
  std::optional<std::string> quantResolution,
  bool debug,
  float bpm);

// Used internally and for testing
std::string OutputEvents(const Events& events);

// Output with as much info as poss
std::string OutputEventsDebug(int tpq, 
  const std::string& bar1Preamble, const Events& events);
}

