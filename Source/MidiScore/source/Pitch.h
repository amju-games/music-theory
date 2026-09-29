#pragma once

#include <string>
#include "KeySig.h"

namespace MidiScore
{
// Return string for the given MIDI pitch.
std::string PitchToString(int pitch);

// Return string for the given MIDI pitch.
// We convert to step/octave/alt format, using the prevailing
//  key sig to decide what to choose for black notes.
std::string PitchToString(int pitch, KeySig ks);
}

