// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include "Accidental.h"
#include "Glyph.h"
#include "KeySig.h"

// * NoteGlyph *
struct NoteGlyph : public Glyph
{
  NoteGlyph() = default;

  // Construct glyph from inpupt token and position in bar.
  NoteGlyph(const std::string& inputToken, int order) :
    Glyph(inputToken, order) {}

  // Generate TIME special glyphs, (for animation)
  std::string TimeBefore() const override;
  std::string TimeAfter() const override;

  std::string ToString() const override;

  void SetPitch(int pitch_)
  {
    pitch = pitch_;
  }

  void SetDisplayNameForBeamedNote();

  // Use input token and state to generate output text for this glyph.
  // TODO Doesn't need param and can set displayGlyphName directly
  std::string GetGlyphOutputStr(std::string s) const override;

  // Calc accidental, given pitch and key sig
  void CalcAccidental(KeySig ks);

  // Adjust accidental calculated above, based on the most recent
  //  accidental set on this stave line
  void AdjustAccidental(Accidental previousAcc);

  std::string GetAccidentalStr() const;

  std::string GetStaccatoStr() const;


  int pitch = DEFAULT_PITCH;

  Accidental m_accidental = Accidental::ACCIDENTAL_NONE;

  // TODO
  // Stem* m_stem;
};
