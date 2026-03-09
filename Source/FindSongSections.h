#pragma once

#include <optional>
#include "NoteEvent.h"

namespace Amju
{
using Section = std::pair<int, int>;
using Sections = std::vector<Section>;

// ** FindSongSections **
// Identify sections in a song, defined by a sequence of NoteEvents.
// Each section is defined by a pair of ints, [first, last). The
//  ints are the IDs of the events in the section, (which is the same as
//  their indices in the NoteEvents vector -- we need the redundancy).
Sections FindSongSections(const NoteEvents& song);

// Handy function to get min and max note pitches in the given section
//  of the given song.
// Return value is (min, max), if the section contains notes. 
std::optional<std::pair<int, int>> FindMinMaxPitchInSection(
  const Section&, const NoteEvents&);
}

