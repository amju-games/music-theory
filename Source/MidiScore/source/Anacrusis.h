#pragma once

#include <optional>
#include <string>

namespace MidiScore
{
int GetAnacrusisTicks(std::optional<std::string> anacrusis, int tpq);
}

