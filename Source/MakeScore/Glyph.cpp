// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include "Glyph.h"
#include "Switch.h"

std::string Glyph::ToString() const
{
  return displayGlyphName + ", " + Str(x) + ", " + Str(y) +
    ", " + Str(scaleX) + ", " + Str(scaleY);
}

std::string Glyph::GetGlyphOutputStr(std::string s) const
{
  std::string out = "UNKNOWN";

  bool dot = Contains(s, '.');
  Remove(s, '.');

  if (s == INPUT_TOKEN_CROTCHET) out = "crotchet";
  else if (s == INPUT_TOKEN_QUAVER) out = "quaver";
  else if (s == INPUT_TOKEN_SEMIQUAVER) out = "semiquaver";
  else if (s == INPUT_TOKEN_MINIM) out = "minim";
  else if (s == INPUT_TOKEN_SEMIBREVE) out = "semibreve";

  if (dot)
  {
    // TODO Add a separate dot glyph, don't have a combinatorial number of compound glyphs
    out = "dotted-" + out;
    // Raised dot if glyph is on a line
    bool onLine = (m_staveLine % 2 == 0);
    if (onLine)
    {
      out += "-raised-dot";
    }
  }

  return out;
}

Glyph::Glyph(const std::string& inputToken, int order_) :
  order(order_),
  realGlyphName(inputToken)
{
  // Calc output (display) text later, but if we want to hide this glyph
  //  for a question (e.g. 'what is this note?'), do it now.
  HandleStar();
}

void Glyph::HandleStar()
{
  if (Contains(realGlyphName, '*'))
  {
    Remove(realGlyphName, '*');
    displayGlyphName = '*';
  }
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
    float t = timeval + startTime;
    if (m_switches & SW_STACCATO)
    {
      t = timeval * 0.5f + startTime; // halve length of note
    }
    res += "TIME, " + Str(start) + ", " + Str(t) + " ; ";
  }
  return res;
}

std::string Glyph::TimeAfter() const
{
  std::string res;

  bool yesTime = (timeval > 0);
  if (yesTime)
  {
    // Cancel time for subsequent glyphs (but postprocess to strip out
    //  unnecessary cancellations)
    res += " ; TIME, -1, -1";
  }
  return res;
}
