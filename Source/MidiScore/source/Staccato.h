#pragma once

#include "Event.h"

namespace MidiScore
{
// Run staccato pass over the given events.
// Look for notes followed by a gap, the same duration as the note.
// If found, double the note duration and mark as staccato.
int StaccatoPass(Events& events);
}

