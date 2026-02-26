#pragma once

// * MakeScore *
// Juliet Colman 2026

#include <memory>
#include <string>
#include "BeamGroup.h"
#include "Consts.h"
#include "IGlyph.h"
#include "StemDir.h"

class Quad;

enum class Tail
{
  TAIL_NONE,
  TAIL_Q,
  TAIL_QQ,
  TAIL_QQQ,
  // ...
};

class Stem : public IGlyph
{
public:
  enum class LengthType
  {
    NONE, // e.g. semibreve
    VARIABLE, // length is non-standard, for beamed notes and chords
    STANDARD, // for crotchets, minims, non-beamed qs etc.
  };
  void SetLengthType(LengthType lt) { m_lengthType = lt; }
  
  void SetDirection(StemDir d) { m_direction = d; }
  StemDir GetDirection() const { return m_direction; }

  std::string ToString() const override;
  std::string CommentString() const override;

  void SetIsChord() { m_isChord = true; }

  // For chords, set the min and max stave lines, i.e. the positions of
  //  the lowest and highest notes in the chord.
  void SetMinMaxStaveLines(int minStave, int maxStave);

  void SetTailFromTimeType(TimeType);

  void SetBeamGroup(PBeamGroup& bg) { m_beamGroup = bg; }

private:
  // Make a quad for this stem, if it's variable length.
  Quad MakeQuad() const;

  std::string TailString() const;
  std::string TailUpString() const;
  std::string TailDownString() const;

private:
  StemDir m_direction = StemDir::NONE;
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

  // Tail type -- is overridden if note/chord is part of a beam group.
  Tail m_tail;

  PBeamGroup m_beamGroup;
};

