// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <cassert>
#include "RestGlyph.h"
#include "Suppress.h"

bool IsImmediateRest(const std::string& token)
{
  // R means whole-bar rest
  return token == "r" || token == "R";
}

bool IsWholeBarRest(const std::string& token)
{
  return token == "R";
}

bool IsDeferredRest(const std::string& token)
{
  return token == "<r>";
}

RestGlyph::RestGlyph()
{
  // Set y: the default position is the middle of the stave, i.e.
  //  stave line 4 (counting spaces too, from 0 at bottom).
  y = STAVE_LINE_GAP * 2;

  // It would be nice to allow immediate rests to have an optional
  //  stave line param, e.g. `r 8` would position the rest on the top
  //  stave line. TODO
}

RestGlyph::RestGlyph(const std::string& inputToken, bool isWholeBar, int order) :
  Glyph(inputToken, order)
{
  y = STAVE_LINE_GAP * 2; // see above

  // `R` means whole bar rest, but inputToken is the duration string
  //  (which should be a time val tho)
  m_isWholeBar = isWholeBar;
}

std::string RestGlyph::CommentString() const
{
  return "// Rest, value: " + m_times.GetTimeToken() +
    (m_isWholeBar ? ": Whole Bar" : "") + LineEnd();
}

std::string RestGlyph::TimeBefore() const
{
  std::string res;

  const float timeval = m_times.GetNormalisedDuration();
  bool yesTime = (timeval > 0); // why would this be false?
  if (yesTime)
  {
    const float startTime = m_times.GetNormalisedStartTime();
    float start = std::max(MIN_START_TIME, startTime);
    
    float t = timeval + startTime;

    if ((GetSuppressFlags() & META_TIME) == 0)
    {
      res += "TIME, " + Str(start) + ", " + Str(t) + LineEnd();
    }

    if ((GetSuppressFlags() & META_REST) == 0)
    {
      // extra meta data for client to identify rests
      res += "REST_ON, " +
        Str(x)  + ", " + 
        Str(y) + 
        LineEnd(); 
    }
  }
  return res;
}

std::string RestGlyph::TimeAfter() const
{
  std::string res;

  bool yesTime = (m_times.GetTimeValue() > 0); // why would this be false?
  if (yesTime)
  {
    if ((GetSuppressFlags() & META_REST) == 0)
    {
      // extra meta data for client to identify rests
      res += "REST_OFF" + LineEnd(); 
    }

    if ((GetSuppressFlags() & META_TIME) == 0)
    {
      // Cancel time for subsequent glyphs (but postprocess to strip out
      //  unnecessary cancellations)
      res += "TIME, -1, -1";
    }
  }
  return res;
}

std::string RestGlyph::ToString() const
{
  std::string res;

  // Add meta data for timing before and after - this is
  //  for animation and MIDI events. 
  res += TimeBefore();

  res += GetGlyphOutputStr() + ", " + CoordString() +
    AddScaleStringIfRequired() + LineEnd();

  if (m_times.IsDotted())
  {
    // TODO These output strings should be Consts.
    std::string dotType = m_staveLine % 2 == 0 ? "raised-dot" : "reg-dot";
    res += dotType + ", " + CoordString() + 
       AddScaleStringIfRequired() +
       LineEnd();
  }

  res += TimeAfter();

  return res;
}

std::string RestGlyph::GetGlyphOutputStr() const
{
  // TODO Not sure about these strings, they should be Consts.

  auto tt = m_times.GetTimeType();
  switch (tt)
  {
  case TimeType::NOT_SET:
  case TimeType::ERROR:
    return "** ERROR **"; // TODO error strings should be Consts, with description

  case TimeType::QQQ: 
  case TimeType::DOTTED_QQQ:
    return "rest-3";

  case TimeType::SEMIQUAVER: 
  case TimeType::DOTTED_SEMIQUAVER:
    return "rest-semiquaver";

  case TimeType::QUAVER: 
  case TimeType::DOTTED_QUAVER:
    return "rest-quaver";

  case TimeType::CROTCHET: 
  case TimeType::DOTTED_CROTCHET:
    return "rest-crotchet";

  case TimeType::MINIM: 
  case TimeType::DOTTED_MINIM:
    return "rest-minim";

  case TimeType::SEMIBREVE: 
  case TimeType::DOTTED_SEMIBREVE:
    return "rest-semibreve";
  }
  return "UNKNOWN!";
}

