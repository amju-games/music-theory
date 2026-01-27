// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#pragma once

#include <string>
#include "Event.h"

namespace MidiScore
{
enum class KeySig
{
  KS_SHARP_0,
  KS_SHARP_1,
  KS_SHARP_2,
  KS_SHARP_3,
  KS_SHARP_4,
  KS_SHARP_5,
  KS_SHARP_6,
  KS_SHARP_7,
  KS_FLAT_0,
  KS_FLAT_1,
  KS_FLAT_2,
  KS_FLAT_3,
  KS_FLAT_4,
  KS_FLAT_5,
  KS_FLAT_6,
  KS_FLAT_7,
};

std::string KeySigString(KeySig ks);

KeySig GuessKeySig(const Events& events, bool preferFlatKey = false);
}

