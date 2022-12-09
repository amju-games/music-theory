// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include "Accidental.h"
#include "IGlyph.h"
#include "KeySig.h"
#include "TimeValue.h"
#include "Utils.h"

struct Tie;

// * Glyph *
// Note or rest glyph, corresponding to one input token, and one output
//  (compound) glyph.
struct Glyph : public IGlyph
{
  Glyph() = default;

  // Construct glyph from inpupt token and position in bar.
  Glyph(const std::string& inputToken, int order);

  // Replace display (output) glyph name with a star if required.
  void HandleStar();

  std::string TimeBefore() const;
  std::string TimeAfter() const;

  std::string ToString() const override;

  void SetPitch(int pitch_)
  {
    pitch = pitch_;
  }

  // Set stave line for this glyph (notes only)
  void SetStaveLine(int staveLine) { m_staveLine = staveLine; }

  void SetTimeVal(float timeval_) { timeval = timeval_; }

  void SetDisplayNameForBeamedNote();

  // Call to set this glyph as the left hand side of the given tie
  void SetTieLeft(Tie* tie) { m_tieLeft = tie; }

  // Set this glyph as the right hand side of the given tie
  void SetTieRight(Tie* tie) { m_tieRight = tie; }

  void SetSwitches(int switches) { m_switches = switches; }

  // Use input token and state to generate output text for this glyph.
  // TODO Doesn't need param and can set displayGlyphName directly
  std::string GetGlyphOutputStr(std::string s) const;

  // Calc accidental, given pitch and key sig
  void CalcAccidental(KeySig ks);

  // Adjust accidental calculated above, based on the most recent
  //  accidental set on this stave line
  void AdjustAccidental(Accidental previousAcc);

  std::string GetAccidentalStr() const;

  int order = 0; // horiz position in bar 

  // Two glyph names. E.g. we have a quaver, but it's drawn using a 
  //  crotchet glyph because it's beamed. So its 'real' name is 'q',
  //  (i.e. the input token) but its display name is 'crotchet'.
  // Also we can set the display name to "*" for questions like 'what is the note?'.
  std::string displayGlyphName;
  std::string realGlyphName;

  // Time value for this glyph, i.e. its duration.
  TimeValue timeval = 0;

  // Start time is the accumulated time values of all preceding glyphs.
  TimeValue startTime = 0;

  int pitch = DEFAULT_PITCH;

  // Points to tie - we are the LEFT glyph of the tie
  Tie* m_tieLeft = nullptr;

  // Points to tie - we are the RIGHT glyph of the tie
  Tie* m_tieRight = nullptr;

  // Line within stave on which this glyph sits - 2 is the middle line.
  int m_staveLine = 2;

  // Accidental, to be added to the left of the glyph
  Accidental m_accidental = Accidental::ACCIDENTAL_NONE;
};
