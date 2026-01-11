#pragma once

#include <vector>
#include "Glyph.h"
#include "Pitch.h"

// Duration of every note in the chord is the last prevailing duration.
using Chord = std::vector<Pitch>;

class ChordGlyph : public Glyph
{
public:

protected:
  GlyphVec m_glyphs;
  Chord m_pitches;
};

