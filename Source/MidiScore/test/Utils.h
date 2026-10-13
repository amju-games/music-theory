#pragma once

#include "Event.h"

namespace MidiScore
{
// Util functions for tests

// Create one note event 
Event n(int pitch, int start, int duration, int tpq);

// Create a rest event
Event r(int start, int duration, int tpq);

// Create a barline event
Event barline(int start, int tpq);
}

