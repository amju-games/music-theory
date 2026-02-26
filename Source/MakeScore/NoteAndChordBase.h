#pragma once

#include "BeamGroup.h"
#include "Glyph.h"

class NoteAndChordBase : public Glyph
{
public:
  NoteAndChordBase() = default;

  NoteAndChordBase(const std::string& token, int orderInBar) :
    Glyph(token, orderInBar) {}

  // True if note is < a crotchet
  virtual bool IsBeamable() const { return false; }
  
  // Return shared ptr to beam group, if in a beam group, else nullptr.
  PBeamGroup GetBeamGroup() { return m_beamGroup; }
  
  // Called when setting up beams
  void SetBeamGroup(PBeamGroup bg) { m_beamGroup = bg; }

protected:
  PBeamGroup m_beamGroup;
};

