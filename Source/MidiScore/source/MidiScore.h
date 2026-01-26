#pragma once

#include <string>

namespace smf
{
class MidiFile;
}

namespace MidiScore
{
std::string ToString(smf::MidiFile& mf);
}

