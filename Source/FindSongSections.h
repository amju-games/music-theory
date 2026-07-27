#pragma once

#include <optional>
#include <ostream>
#include "GuiMusicScore.h"
#include "NoteEvent.h"

namespace Amju
{
// * Section *
// Each section is defined by a pair of ints, [first, last). The
//  ints are the IDs of the events in the section, (which is the same as
//  their indices in the NoteEvents vector -- we need the redundancy).
using Section = std::pair<int, int>;

// * Sections *
// A song probably contains multiple sections, but should have at least 
//  one - the entire song.
using Sections = std::vector<Section>;

inline std::ostream& operator<<(std::ostream& os, const Section& s)
{
  return os << "[" << s.first << ", " << s.second << ")";
}

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

// We can manually define section ends in the game round file.
// This function adds extra sections to the given list, using the list of
//  bar numbers (zero-based). The end of each bar number is the end of
//  a section.
// TODO Make this work: find events in bars!
void AddGameRoundSections(Sections& sections, const std::vector<int>& bars,
  const NoteEvents& song, const BeatVec& beats);

// Helper: given existing sections and a list of (possibly) additional IDs,
//  split existing sections and add new sections as appropriate.
void InsertNewSections(Sections& sections, const std::vector<int>& ids,
  const NoteEvents& song);
}

