// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include "Accidental.h"
#include "Consts.h"
#include "Glyph.h"
#include "KeySig.h"
#include "Pitch.h"
#include "Stem.h"

// * NoteGlyph *
struct NoteGlyph : public Glyph
{
  NoteGlyph() = default;

  // Construct glyph from input token and position in bar.
  NoteGlyph(const std::string& inputToken, int order) :
    Glyph(inputToken, order) {}

  // Generate TIME special glyphs, (for animation)
  std::string TimeBefore() const;
  std::string TimeAfter() const;

  std::string ToString() const override;

  // Set up stem from stave line etc
  void SetStem();

  void SetPitch(Pitch p)
  {
    m_pitch = p;
  }

  // Use input token and state to generate output text for this glyph.
  std::string GetGlyphOutputStr() const;

  // Calc y-pos of note, i.e. position on stave.
  void CalcY(KeySig keySig, Clef clef);

  // Calc stave line, used in CalcY above.
  // See comment in Glyph.h explaining stave lines.
  static int CalcStaveLine(KeySig keySig, Clef clef, const Pitch& pitch);

  // Calc accidental, given key sig
  // Returns accidental calculated, and sets member.
  Accidental CalcAccidental(KeySig ks);
  // Internal functions - public for testing
  static Accidental CalcAccidentalFromStepOctAlter(KeySig ks, Pitch pitch);
  static Accidental CalcAccidentalFromMidi(KeySig ks, Pitch pitch);

  // Get the accidental - must first be calculated by calling CalcAccidental.
  Accidental GetAccidental() const { return m_accidental; }

  // Adjust accidental calculated above, based on the most recent
  //  accidental set on this stave line
  void AdjustAccidental(Accidental previousAcc);

  // Get the final output string for the accidental for this note.
  std::string GetAccidentalStr() const;

  // Get the final output string for the staccato dot for this note.
  std::string GetStaccatoStr() const;

  std::string CommentString() const override;

  // Return true if not a semibreve - TODO other cases?
  bool ShouldHaveStem() const;

  // Call if this note is in a chord and because of overlaps with other
  //  notes, should be offset in x.
  // Set to 0 (no overlap) or +1 or -1 depending on stave direction.
  void SetOverlapOffset(int overlapOffset) { m_overlapOffset = overlapOffset; }

  // Call to offset the accidental for this note. 
  // As accidentals can overlap in y, we might need to offset by more
  //  than one offset distance. 
  // offsets: the number of offset distances we should move.
  void SetAccidentalOverlapOffset(int offsets) { m_accidentalOverlapOffsets = offsets; }

private:
  Pitch m_pitch;

  Accidental m_accidental = Accidental::ACCIDENTAL_NONE;

  // If true, this note overlaps another in a chord, and so 
  //  should be offset in x.
  int m_overlapOffset = 0;

  // Number of offset distances we should move to avoid an overlap.
  int m_accidentalOverlapOffsets = 0;

  int m_volume = 127;

  Stem m_stem;
};
