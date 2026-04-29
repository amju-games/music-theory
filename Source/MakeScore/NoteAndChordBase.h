#pragma once

#include "BeamGroup.h"
#include "Glyph.h"
#include "Stem.h"

class NoteAndChordBase : public Glyph
{
public:
  NoteAndChordBase() = default;

  NoteAndChordBase(const std::string& token, int orderInBar) :
    Glyph(token, orderInBar) {}

  // Set stem dir: this is for beaming.
  // If beamed, the stem dir will be set by the beam group.
  void SetStemDirection(StemDir);

  // Calculate stem dir for note or chord; set internal stem dir
  //  and return that value for convenience.
  virtual StemDir DecideStemDirection() { return StemDir::NONE; }

  void SetStemLength(float length) { m_stem.SetLength(length); }

  const Stem& GetStem() const { return m_stem; }

  // For beaming, get the stave line of the note. If a chord,
  //  get the stave line of the note closest to the beam given
  //  the direction of all stems in the beam group.
  // This is used for the 'majority vote', but can then be
  //  overridden by the Beam Group.
  virtual int GetStaveLineForBeam(StemDir) { return 0; }

  // Get beam level: 1 for q, 2 for qq, etc.
  virtual int GetBeamLevel() const { return 1; }

  void SetIsBeamed() { m_isBeamed = true; }

  bool IsBeamed() const { return m_isBeamed; }

  virtual int GetNumAccidentals() const { return 0; }

protected:
  bool m_isBeamed = false;
  Stem m_stem;
};

