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
#include "LedgerLine.h"
#include "NoteAndChordBase.h"
#include "Pitch.h"
#include "Stem.h"

// * NoteGlyph *
struct NoteGlyph : public NoteAndChordBase
{
  NoteGlyph() = default;

  // Construct glyph from input token and position in bar.
  NoteGlyph(const std::string& inputToken, int order) :
    NoteAndChordBase(inputToken, order) {}

  // Output final note data
  std::string ToString() const override;

  // Output meta data, for animating and playing notes: start time,
  //  note events, etc.
  std::string TimeBefore() const;
  std::string TimeAfter() const;

  std::string CommentString() const override;

  bool IsBeamable() const override;

  // Functions used to build up full string - these can be called from
  //  chord, so not private.
  // These are the final output strings, with coords, scale factors.
  std::string DotString() const;
  std::string AccidentalString() const;
  std::string LedgerLinesString(LedgerLineWidth width) const;
  std::string StemString() const;
  std::string NoteHeadString() const;
  std::string StaccatoString() const;

  // Set up stem: called when single note added.
  // For chords, the stems of the individual notes are not set.
  void SetStem();

  void SetIsPartOfChord(bool isPartOfChord) { m_isPartOfChord = isPartOfChord; }

  void SetPitch(const Pitch& p) { m_pitch = p; }

  // Calc y-pos of note, i.e. position on stave.
  void CalcY(KeySig keySig, Clef clef);

  // Calc stave line, used in CalcY above.
  // Stave lines are ints; spaces count. Bottom line of stave is zero,
  //  top line is 8, (because of spaces in between). This numbering
  //  is independent of the clef in use.
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

  // Return true if not a semibreve - TODO other cases?
  bool ShouldHaveStem() const;

  // Call if this note is in a chord and because of overlaps with other
  //  notes, should be offset in x.
  // Set to 0 (no offset) or +1 (right) or -1 (left) depending on stave direction.
  // TODO Make sure this works with SBs.
  void SetOverlapOffset(int overlapOffset) { m_overlapOffset = overlapOffset; }

  // Called by chord to offset the accidental for this note. 
  // As accidentals can overlap in y, we might need to offset by more
  //  than one offset distance. 
  // offsets: the number of offset distances we should move.
  void SetAccidentalOverlapOffset(int offsets) { m_accidentalOverlapOffsets = offsets; }

protected:
  // Decide ledger line width
  // For a single note, ledger line width can be for a sb or other note.
  LedgerLineWidth DecideLedgerLineWidth() const;

  // Get the glyph name for the accidental for this note.
  std::string GetAccidentalStr() const;

  // Use input token and state to generate output text for this glyph.
  std::string GetGlyphOutputStr() const;

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

  // If true, this note is part of a chord.
  bool m_isPartOfChord = false;
};
