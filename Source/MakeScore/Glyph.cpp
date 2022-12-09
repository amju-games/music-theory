// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include "Glyph.h"

// Generate GuiMusicScore glyph string from input token
static std::string GetStr(std::string s)
{
  bool dot = Contains(s, '.');
  Remove(s, '.');
  bool rest = Contains(s, 'r');
  Remove(s, 'r');

  std::string out = "UNKNOWN";
  if (s == INPUT_TOKEN_CROTCHET) out = "crotchet";
  else if (s == INPUT_TOKEN_QUAVER) out = "quaver";
  else if (s == INPUT_TOKEN_SEMIQUAVER) out = "semiquaver";
  else if (s == INPUT_TOKEN_MINIM) out = "minim";
  else if (s == INPUT_TOKEN_SEMIBREVE) out = "semibreve";

  if (rest)
  {
    out = "rest-" + out;
  }
  else if (s != "sb")
  {
    out += "-up"; // TODO up/down flag
  }

  if (dot)
  {
    // TODO raised dot if glyph is on a line
    out = "dotted-" + out + "-raised-dot";
  }
  return out;
}

Glyph::Glyph(const std::string& inputToken, int order_) :
  displayGlyphName(GetStr(inputToken)), realGlyphName(inputToken),
  order(order_)
{
  HandleStar();
}

void Glyph::SetDisplayNameForBeamedNote()
{
  // E.g. "q" or "qq" -> "crotchet-up" for a beamed quaver.
  // Take dottedness into account.

  bool dot = Contains(realGlyphName, '.');
  displayGlyphName = GetStr(dot ? "c." : "c");
}

void Glyph::HandleStar()
{
  if (Contains(realGlyphName, '*'))
  {
    Remove(realGlyphName, '*');
    displayGlyphName = '*';
  }
}

std::string Glyph::ToString() const
{
  // Add special glyphs for timing before and after - this is
  //  for animation and MIDI events. 
  std::string res = TimeBefore();

  res += displayGlyphName + ", " + Str(x) + ", " + Str(y) +
    ", " + Str(scale) + ", " + Str(scale);

  res += TimeAfter();

  return res;
}

std::string Glyph::TimeBefore() const
{
  std::string res;

  bool yesTime = (timeval > 0);
  if (yesTime)
  {
    float start = startTime;
    if (start == 0)
    {
      start = 0.01f; // so first glyph is not highlighted until anim starts
    }
    res += "TIME, " + Str(start) + ", " + Str(timeval + startTime) + " ; ";

    if (!IsRest(realGlyphName) && !m_tieRight)
    {
      // Output MIDI note event, unless on RHS of a tie
      res += "NOTE_ON, " + Str(pitch) + ", " + Str(start) + " ; ";
    }
  }
  return res;
}

std::string Glyph::TimeAfter() const
{
  std::string res;

  bool yesTime = (timeval > 0);
  if (yesTime)
  {
    if (!IsRest(realGlyphName) && !m_tieLeft)
    {
      // Output MIDI note off event, unless the note is on LHS of a tie,
      //  in which case it will last longer.
      // Follow chain of ties back to start of tie, to get total length.
      // TODO 
      res += " ; NOTE_OFF, " + Str(pitch) + ", " + Str(timeval + startTime);
    }

    // Cancel time for subsequent glyphs (but postprocess to strip out
    //  unnecessary cancellations)
    res += " ; TIME, -1, -1";
  }
  return res;
}
