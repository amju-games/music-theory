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

  // Stems: just for notes and chords.
  // If beamed, the stem dir will be set by the beam group.
  virtual void SetStemDirection(StemDir) {}

  virtual StemDir DecideStemDirection() { return StemDir::NONE; }

protected:
  PBeamGroup m_beamGroup;
};

