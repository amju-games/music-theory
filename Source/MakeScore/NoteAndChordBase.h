#pragma once

#include "BeamGroup.h"
#include "Glyph.h"
#include "StemDir.h"

class NoteAndChordBase : public Glyph
{
public:
  NoteAndChordBase() = default;

  NoteAndChordBase(const std::string& token, int orderInBar) :
    Glyph(token, orderInBar) {}

  // Return shared ptr to beam group, if in a beam group, else nullptr.
  PBeamGroup GetBeamGroup() { return m_beamGroup; }
  
  // Called when setting up beams
  void SetBeamGroup(PBeamGroup bg) { m_beamGroup = bg; }

  // Set stem dir: this is for beaming.
  // If beamed, the stem dir will be set by the beam group.
  virtual void SetStemDirection(StemDir) {}

  // Calculate stem dir for note or chord; set internal stem dir
  //  and return that value for convenience.
  virtual StemDir DecideStemDirection() { return StemDir::NONE; }

  // For beaming, get the stave line of the note. If a chord,
  //  get the stave line of the note closest to the beam given
  //  the direction of all stems in the beam group.
  virtual int GetStaveLineForBeam(StemDir dir) { return 0; }

  // Get beam level: 1 for q, 2 for qq, etc.
  virtual int GetBeamLevel() const { return 1; }

protected:
  PBeamGroup m_beamGroup;
};

