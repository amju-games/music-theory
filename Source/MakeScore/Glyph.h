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

struct Glyph : public IGlyph
{
  Glyph() = default;
  Glyph(const std::string& str_, int order_) :
    displayGlyphName(GetStr(str_)), realGlyphName(str_),
    order(order_)
  {
    HandleStar();
  }

  void HandleStar();

  std::string TimeBefore() const;
  std::string TimeAfter() const;

  std::string ToString() const override
  {
    // Add special glyphs for timing before and after - this is
    //  for animation and MIDI events. 
    std::string res = TimeBefore();

    res += displayGlyphName + ", " + Str(x) + ", " + Str(y) +
      ", " + Str(scale) + ", " + Str(scale);

    res += TimeAfter();

    return res;
  }

  void SetTimeVal(float timeval_) { timeval = timeval_; }

  void SetDisplayNameForBeamedNote()
  {
    // E.g. "q" or "qq" -> "crotchet-up" for a beamed quaver.
    // Take dottedness into account.

    bool dot = Contains(realGlyphName, '.');
    displayGlyphName = GetStr(dot ? "c." : "c");
  }

  // Call to set this glyph as the left hand side of the given tie
  void SetTieLeft(Tie* tie) { m_tieLeft = tie; }

  // Set this glyph as the right hand side of the given tie
  void SetTieRight(Tie* tie) { m_tieRight = tie; }

  int order = 0; // horiz position in bar 

                 // Two glyph names. E.g. we have a quaver, but it's drawn using a 
                 //  crotchet glyph because it's beamed. So its 'real' name is 'q',
                 //  but its display name is 'crotchet'.
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
};
