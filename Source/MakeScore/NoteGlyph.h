// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include "Accidental.h"
#include "Glyph.h"
#include "KeySig.h"
#include "Pitch.h"

// * NoteGlyph *
struct NoteGlyph : public Glyph
{
  NoteGlyph() = default;

  // Construct glyph from input token and position in bar.
  NoteGlyph(const std::string& inputToken, int order) :
    Glyph(inputToken, order) {}

  // Generate TIME special glyphs, (for animation)
  std::string TimeBefore() const override;
  std::string TimeAfter() const override;

  std::string ToString() const override;

  void SetPitch(Pitch p)
  {
    m_pitch = p;
  }

  void SetDisplayNameForBeamedNote();

  // Use input token and state to generate output text for this glyph.
  // TODO Doesn't need param and can set displayGlyphName directly
  std::string GetGlyphOutputStr(std::string s) const override;

  // Calc y-pos of note, i.e. position on stave.
  void CalcY(KeySig keySig, Clef clef);

  // Calc stave line, used in CalcY above.
  // See comment in Glyph.h explaining stave lines.
  static int CalcStaveLine(KeySig keySig, Clef clef, const Pitch& pitch);

  // Calc accidental, given pitch and key sig
  // TODO handle when overriden by specifying step/octave/alter
  // Returns accidental calculated, and sets member.
  Accidental CalcAccidental(KeySig ks);
  // Internal functions - public for testing
  static Accidental CalcAccidentalFromStepOctAlter(Pitch pitch);
  static Accidental CalcAccidentalFromMidi(KeySig ks, Pitch pitch);

  Accidental GetAccidental() const { return m_accidental; }

  // Adjust accidental calculated above, based on the most recent
  //  accidental set on this stave line
  void AdjustAccidental(Accidental previousAcc);

  std::string GetAccidentalStr() const;

  std::string GetStaccatoStr() const;

  std::string CommentString() const override;

private:
  Pitch m_pitch;

  Accidental m_accidental = Accidental::ACCIDENTAL_NONE;

  int m_volume = 127;

  // TODO
  // Stem* m_stem;
};
