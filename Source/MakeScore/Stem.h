#pragma once

// * MakeScore *
// Juliet Colman 2026

#include <memory>
#include <string>
#include "Consts.h"
#include "IGlyph.h"

struct NoteGlyph;
class ChordGlyph;
class Quad;

class Stem : public IGlyph
{
public:
  static Stem CreateNoteStem(std::unique_ptr<NoteGlyph>&);
  static Stem CreateChordStem(std::unique_ptr<ChordGlyph>&);

  enum class LengthType
  {
    NONE, // e.g. semibreve
    VARIABLE, // length is non-standard, for beamed notes and chords
    STANDARD, // for crotchets, minims, non-beamed qs etc.
  };
  void SetLengthType(LengthType lt) { m_lengthType = lt; }
  
  enum class Direction
  {
    NONE, UP, DOWN
  };
  void SetDirection(Direction d) { m_direction = d; }

  std::string ToString() const override;
  std::string CommentString() const override;

  void SetIsChord() { m_isChord = true; }

  // For chords, set the min and max stave lines, i.e. the positions of
  //  the lowest and highest notes in the chord.
  void SetMinMaxStaveLines(int minStave, int maxStave);

  // Make a quad for this stem, if it's variable length.
  Quad MakeQuad() const;

private:
  Direction m_direction = Direction::NONE;
  LengthType m_lengthType = LengthType::NONE;

  // For a single note, the length of the stem is the default, and 
  //  we just output a stem glyph code.
  // For a beamed note, the length is calculated to join the beam.
  // For a chord, this is the 'extra' length beyond the span between
  //  min and max notes in the chord. So this could be the default,
  //  or the 'beam length'.
  // Unit is gap betwen two adjacent stave lines == 1. I.e. not
  //  scaled by coord system distance yet.
  float m_length = DEFAULT_STEM_H;

  // Not a stem type, but rather a flag, because we can have beamed 
  // chords, and chords of semibreves
  bool m_isChord = false; 
  
  int m_minStave = 0;
  int m_maxStave = 0;
};

