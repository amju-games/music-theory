// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include "IGlyph.h"
#include "TimeValue.h"
#include "Utils.h"

struct Tie;

// * Glyph *
// Base class for note and rest glyphs.
struct Glyph : public IGlyph
{
  Glyph() = default;

  // Construct glyph from inpupt token and position in bar.
  Glyph(const std::string& inputToken, int order);

  // Replace display (output) glyph name with a star if required.
  void HandleStar();

  // Generate TIME special glyphs, (for animation)
  virtual std::string TimeBefore() const;
  virtual std::string TimeAfter() const;

  std::string ToString() const override;

  // Set stave line for this glyph (notes and rests, so we can
  //  adjust standard rest position)
  // Stave line is a position on a standard 5-line stave. The bottom line
  //  is at position 0, and the top line is position 8, because the intervening
  //  lines and spaces count. 
  // 
  // Whatever the clef, the bottom stave line is always 0.
  // So the stave line for a particular pitch depends on the clef.
  //
  // Treble clef:
  // 10 -a5- 
  // 9   g5
  // 8  -f5------------------ 
  // 7   e5                
  // 6  -d5------------------
  // 5   c5
  // 4  -b4------------------
  // 3   a4
  // 2  -g4------------------
  // 1   f4
  // 0  -e4------------------
  // -1  d4 
  // -2  c4   -- middle c
  //
  // Bass clef:
  // 10 -c4-  -- middle c
  // 9   b3
  // 8  -a3------------------ 
  // 7   g3                
  // 6  -f3------------------
  // 5   e3
  // 4  -d3------------------
  // 3   c3
  // 2  -b2------------------
  // 1   a2
  // 0  -g2------------------
  // -1  f2 
  // -2  e2 
  void SetStaveLine(int staveLine) { m_staveLine = staveLine; }

  float GetTimeVal() const { return timeval; }
  virtual void SetTimeVal(float timeval_) { timeval = timeval_; }

  float GetStartTime() const { return startTime; }
  virtual void SetStartTime(float st) { startTime = st; }

  // This is for testing and debugging
  void SetTimeValToken(const std::string& token) { timevalToken = token; }

  // Call to set this glyph as the left hand side of the given tie
  void SetTieLeft(Tie* tie) { m_tieLeft = tie; }

  // Set this glyph as the right hand side of the given tie
  void SetTieRight(Tie* tie) { m_tieRight = tie; }

  void SetPerformance(int switches) { m_switches = switches; }

  // Use input token and state to generate output text for this glyph.
  // This is used by note and rest glyphs to output full string of glyphs -- not
  //  used for e.g. time sigs, so there should be another base class -- TODO
  // TODO Doesn't need param and can set displayGlyphName directly.
  virtual std::string GetGlyphOutputStr(std::string s) const;

  int order = 0; // horiz position in bar 

  // Two glyph names. E.g. we have a quaver, but it's drawn using a 
  //  crotchet glyph because it's beamed. So its 'real' name is 'q',
  //  (i.e. the input token) but its display name is 'crotchet'.
  // Also we can set the display name to "*" for questions like 'what is the note?'.
  std::string displayGlyphName;
  std::string realGlyphName;

  // Time value for this glyph, i.e. its duration.
  TimeValue timeval = 0;

  // String duration, for testing
  std::string timevalToken;

  // Start time is the accumulated time values of all preceding glyphs.
  TimeValue startTime = 0;

  // Points to tie - we are the LEFT glyph of the tie
  Tie* m_tieLeft = nullptr;

  // Points to tie - we are the RIGHT glyph of the tie
  Tie* m_tieRight = nullptr;

  // Line within stave on which this glyph sits - 2 is the middle line.
  int m_staveLine = 2;

  // Bit field of flags, e.g. staccato
  int m_switches = 0;
};

using GlyphVec = std::vector<std::unique_ptr<Glyph>>;

