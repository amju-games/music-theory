#pragma once

#include <string>
#include "Event.h"

namespace MidiScore
{
// Format str into a grid format: each bar on a separate line;
//  elements on a line spaced out by start time.
// The idea is to make it easier to visually parse the output.
std::string Gridify(const Events& events);
}

