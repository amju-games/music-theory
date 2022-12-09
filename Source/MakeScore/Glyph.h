// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include "IGlyph.h"
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

  // Use input token and state to generate output text for this glyph.
  // TODO Doesn't need param and can set displayGlyphName directly
  std::string GetGlyphOutputStr(std::string s) const;

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

  int m_staveLine = 2;
};
