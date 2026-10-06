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
// Generate text info about the given midi file.
// mf: the midi file
// track: track number; if nullopt, we report on all tracks.
// anacrusis: optional time value of first bar, using TimeVal notation.
// timeSig: optional time sig, for better reporting.
// allClefs: if true, consider alto and tenor clefs, else just treble
//  and bass.
std::string InfoString(
  smf::MidiFile& mf,
  std::optional<int> track,
  std::optional<std::string> anacrusis,
  std::optional<std::string> timeSig,
  bool allClefs);
}

