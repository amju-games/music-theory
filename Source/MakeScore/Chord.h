#pragma once

#include <vector>
#include "Glyph.h"
#include "Pitch.h"

// Duration of each note in the chord is the last prevailing duration
//  string, so it can change within chord markers.
// TODO Duration is currently a string, should be an enum or type.
using Chord = std::vector<std::pair<Pitch, std::string>>;

// For parsing MakeScore input
bool IsChordStart(const std::string&);
bool IsChordEnd(const std::string&);

// TODO Derive from NoteGlyph? Container of note glyphs??
class ChordGlyph : public Glyph
{
public:

protected:
  GlyphVec m_glyphs;
  Chord m_pitches;
};

