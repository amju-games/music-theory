// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#pragma once

#include <string>
#include "Event.h"

namespace smf
{
class MidiFile;
}

namespace MidiScore
{
std::string ToString(smf::MidiFile& mf);
std::string OutputEvents(const Events& events);
}

