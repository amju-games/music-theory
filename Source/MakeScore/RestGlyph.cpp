// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <cassert>
#include "RestGlyph.h"

std::string RestGlyph::TimeBefore() const
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
    res += "TIME, " + Str(start) + ", " + Str(t) + " ; ";
  }
  return res;
}

std::string RestGlyph::TimeAfter() const
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

std::string RestGlyph::ToString() const
{
  // If we haven't yet created the output text, do it now
  if (displayGlyphName.empty())
  {
    // Argh, cast away constness
    const_cast<std::string&>(displayGlyphName) = GetGlyphOutputStr(realGlyphName);
  }

  // Add special glyphs for timing before and after - this is
  //  for animation and MIDI events. 
  std::string res = TimeBefore();

  res += displayGlyphName + ", " + Str(x) + ", " + Str(y) +
    ", " + Str(scaleX) + ", " + Str(scaleY);

  res += TimeAfter();

  return res;
}

std::string RestGlyph::GetGlyphOutputStr(std::string s) const
{
  bool rest = Contains(s, 'r');
  assert(rest);
  Remove(s, 'r');

  std::string out = Glyph::GetGlyphOutputStr(s);

  out = "rest-" + out;

  return out;

}
