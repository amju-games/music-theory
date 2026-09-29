#pragma once

#include "NoteEvent.h"

// For tests, populate m_id in each element of the given NoteEvent sequence.
// In normal use this happens in GuiMusicScore when we load the events.
void PopulateIdsForNoteEvents(Amju::NoteEvents& noteEvents);

