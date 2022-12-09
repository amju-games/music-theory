// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <iostream>
#include "Bar.h"
#include "Flag.h"

void Bar::CopyState(const Bar& b)
{
  SetStaveType(b.m_staveType);
  SetScale(b.m_scale);
  // Copy time sig over to next bar
  SetTimeSig(b.GetTimeSig());
  // Copy key sig over
  SetKeySig(b.GetKeySig());
  // Copy clefs over
  for (int s = 0; s < MAX_NUM_STAVES; s++)
  {
    m_currentClef[s] = b.m_currentClef[s];
  }
}

void Bar::SetKeySig(KeySig ks)
{
  m_keySig = ks;
}

KeySig Bar::GetKeySig() const
{
  return m_keySig;
}

void Bar::SetTimeSig(TimeSig ts)
{
  m_timeSig = ts;
}

TimeSig Bar::GetTimeSig() const
{
  return m_timeSig;
}

// Return the total time for the bar.
// The time val of all the glyph members should add up to this.
TimeValue Bar::GetDuration() const
{
  switch (m_timeSig)
  {
  case TimeSig::TIME_SIG_NONE:
    // Add up duration of all sequential glyphs
    {
      float d = 0;
      for (auto& g : m_glyphs)
      {
        d += g->timeval;
      }
      return d;
    }

  case TimeSig::TIME_SIG_COMMON:
  case TimeSig::TIME_SIG_FOUR_FOUR:
    return 4.f * TIMEVAL_CROTCHET;

  case TimeSig::TIME_SIG_THREE_FOUR:
    return 3.f * TIMEVAL_CROTCHET;

  case TimeSig::TIME_SIG_CUT_COMMON:
  case TimeSig::TIME_SIG_TWO_FOUR:
    return 2.f * TIMEVAL_CROTCHET;
  }
  return 0;
}

float Bar::CalcNormalisedTimes(float totalDuration, float start)
{
  // Normalise glyph durations, and accumulate to get start times.
  for (auto& g : m_glyphs)
  {
    g->timeval /= totalDuration;
    g->startTime += start;
    start += g->timeval;
  }
  return start;
}

void Bar::SetScale(float scale)
{
  m_scale = scale;
}

void Bar::CalcGlyphY(Glyph* gl, int pitch) const
{
  switch (m_staveType)
  {
  case StaveType::STAVE_TYPE_NONE:
  case StaveType::STAVE_TYPE_RHYTHM:
    gl->y = DEFAULT_HEIGHT;
    break;

  case StaveType::STAVE_TYPE_SINGLE:
  {
    // Y-position for MIDI notes starting from MIDI 0, with C at y = 0.
    // y = 0 corresponds to the bottom line of the stave.
    const int Y_POS[12] = 
    {
      0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6
    };

    // Add this offset to the y position, setting y to the correct
    //  position on the stave. E.g. for treble clef, middle C should be
    //  at y = -2, i.e. two stave positions below the bottom line.
    const int CLEF_OFFSET[4] = 
    {
      -2, // treb
      10, // bass
       4, // alto
       6, // tenor 
    };
    // Use last clef set for this stave
    int clef = static_cast<int>(m_currentClef[m_currentStave]);
    int staveLine = Y_POS[pitch % 12] + CLEF_OFFSET[clef];
    // Use the octave to shunt note up or down
    int octave = (pitch / 12 - 5) * 7; // so middle C is 0
    staveLine += octave;
    float y = static_cast<float>(staveLine) * 0.05f;
    // TODO Offset y for stave > 1
    gl->y = y;
    gl->SetStaveLine(staveLine);
    break;
  }
  case StaveType::STAVE_TYPE_DOUBLE:
    break; // TODO
  }
}

void Bar::AddGlyph(const std::string& s, int pitch)
{
  // TODO Split into note and rest glyph types?
  bool rest = Contains(s, 'r');

  int order = static_cast<int>(m_glyphs.size());

  Glyph* gl = new Glyph(s, order);
  gl->SetScale(m_scale);

  if (!rest)
  {
    gl->SetPitch(pitch);

    // TODO Calc y, using current pitch, stave, and clef. 
    // TODO Add accidental if required, by checking current key sig.
    CalcGlyphY(gl, pitch);
  }

  // Set duration for this musical symbol
  gl->SetTimeVal(GetTimeVal(s));

  m_glyphs.push_back(std::unique_ptr<Glyph>(gl));
}

void Bar::SetClef(Clef clef)
{
  // Has clef changed? If so, output a mini-clef at the end of the bar.
  // Not if this is the first bar though???? Not sure about this - TODO
  // I think we need to add the mini-clef to the PREVIOUS bar.
  if (    clef != m_currentClef[m_currentStave]
      && !m_isFirstBarOfLine) // ?
  {
    m_yesOutputMiniClef = true;
  }
  m_currentClef[m_currentStave] = clef;
}

void Bar::AddTimeSig(const std::string& s)
{
  SetTimeSig(::GetTimeSig(s));

  Glyph* gl = new TimeSigGlyph(s);
  gl->SetScale(m_scale);
  m_timeSigGlyph = std::unique_ptr<Glyph>(gl);
}

void Bar::AddBeam(const std::string& s)
{
  // Beams go between 2 glyphs.
  // Currently we always beam 2 adjacent glyphs, i.e. from n-1 to n
  int n = static_cast<int>(m_glyphs.size());

  // Flags are attached to one glyph, and can go left or right.

  // Level is quaver/semi etc
  m_beams.push_back(std::unique_ptr<Beam>(
    new Beam(BeamLevel::BEAM_LEVEL_1, n - 1, n)));

  if (s == "--" || s == "-")
  {
    // Beam 2 quavers, we are done.
  }
  else if (s == "==" || s == "=")
  {
    // 2 semiquavers, add 2nd beam
    m_beams.push_back(std::unique_ptr<Beam>(
      new Beam(BeamLevel::BEAM_LEVEL_2, n - 1, n)));
  }
  else if (s == "-=")
  {
    // Semiq flag, attached to right stem
    m_beams.push_back(std::unique_ptr<Beam>(
      new Flag(BeamLevel::BEAM_LEVEL_2, n - 1, n, false)));
  }
  else if (s == "=-")
  {
    // Semiq flag, attached to left stem
    m_beams.push_back(std::unique_ptr<Beam>(
      new Flag(BeamLevel::BEAM_LEVEL_2, n - 1, n, true)));
  }
}

std::string Bar::ToString(bool oneLine)
{
  std::string res;

  int numStaves = 1; // TODO 
  // Clef for each stave, if first bar of line, and single or double
  //  stave - not if no stave or just rhythm line
  if (YesShowClefAtFrontOfBar())
  {
    for (int s = 0; s < numStaves; s++)
    {
      float x = 0;
      float y = 0.5f; // Hmm, why do we need to offset in y? 
      y += s * DOUBLE_STAVE_DISTANCE;
      res += GetClefOutputString(m_currentClef[s], s, x, y, m_scale) + 
        LineEnd(oneLine);
    }
  }

  // Key sig
  if (m_isFirstBarOfLine)
  {
    for (int s = 0; s < numStaves; s++)
    {
      float x = 0;
      if (YesShowClefAtFrontOfBar()) // always true if showing key sig?
      {
        x = 0.45f; // TODO TEMP TEST
      }
      float y = 0.5f; // Hmm, why do we need to offset in y? 
      y += s * DOUBLE_STAVE_DISTANCE;
      res += GetKeySigOutputString(m_keySig, m_currentClef[s], s, x, y, m_scale) + 
        LineEnd(oneLine);
    }
  }  

  // Optional time sig
  if (m_timeSigGlyph)
  {
    res += m_timeSigGlyph->ToString() + LineEnd(oneLine);
  }

  // Set display names for beamed (semi)quavers etc. 
  for (auto& b : m_beams)
  {
    m_glyphs[b->left]->SetDisplayNameForBeamedNote();
    m_glyphs[b->right]->SetDisplayNameForBeamedNote();
  }

  for (auto& g : m_glyphs)
  {
    res += g->ToString() + LineEnd(oneLine);
  }

  for (auto& b : m_beams)
  {
    b->SetScale(m_scale);
    res += b->ToString() + LineEnd(oneLine);
  }

  // Bar lines
  res +=
    "bar-line, " + Str(m_x) + ", " + Str(m_y) +
    ", " + Str(m_scale) + ", " + Str(m_scale) +
    LineEnd(oneLine);

  res +=
    "bar-line, " + Str(m_x + m_width) + ", " + Str(m_y) +
    ", " + Str(m_scale) + ", " + Str(m_scale) +
    LineEnd(oneLine);

  return res;
}

bool Bar::YesShowClefAtFrontOfBar() const
{
  return (m_isFirstBarOfLine &&
          m_staveType != StaveType::STAVE_TYPE_NONE &&
          m_staveType != StaveType::STAVE_TYPE_RHYTHM);
}

// Get number of distinct glyphs horizontally
// (e.g. a chord is only one 'glyph' as all the notes take up only 
//  one horizontal space/share the same stem)
int Bar::GetGlyphCount() const
{
  int glyphCount = static_cast<int>(m_glyphs.size());
  if (YesShowClefAtFrontOfBar())
  {
    glyphCount++; // clef
  }
  if (m_timeSigGlyph)
  {
    glyphCount++;
  }
  return glyphCount;
}

void Bar::SetWidth(int totalNumGlyphs, float pageWidth)
{
  int glyphCount = GetGlyphCount();

  m_width = static_cast<float>(glyphCount) /
    static_cast<float>(totalNumGlyphs) * pageWidth / m_scale;
}

float Bar::GetWidth() const
{
  return m_width;
}

float Bar::GetKeySigWidth() const
{
  const float AW = 0.15f; // width of one accidental glyph
  if (m_keySig >= KEYSIG_0_FLAT)
  {
    // Flat
    return (m_keySig - KEYSIG_0_FLAT) * AW;
  }
  else
  {
    // Sharp
    return (m_keySig - KEYSIG_0_SHARP) * AW;
  }
}

// x is the left edge of the bar.
// From this and the width, we can set the final x-coord of each glyph.
// y is an offset added to the y-coord of each glyph (all the same for
//  rhythm scores). 
void Bar::SetPos(float x, float y)
{
  m_x = x;
  m_y = y;

  // xoff is distance from left bar line to first glyph, and also distance
  //  from last glyph to right bar line.
  float numGlyphs = static_cast<float>(GetGlyphCount());
  float xoff = m_width / (numGlyphs + 1.0f);
  float w = 0;

  // Displaying clef at front of this bar?
  if (YesShowClefAtFrontOfBar())
  {
    // Need space for clef, so shunt everything right
    const float CLEF_WIDTH = 0.45f; // ?
    x += CLEF_WIDTH;

    // Also we must be outputting key sig
    x += GetKeySigWidth();

    // Add a bit extra so there is a small space before the time sig
    const float EXTRA_SPACE = 0.1f;
    x += EXTRA_SPACE;
  }

  // Add more for keysig
  // TODO

  if (numGlyphs > 1)
  {
    w = (m_width - 2 * xoff) / (numGlyphs - 1.0f);
    if (m_timeSigGlyph)
    {
      const float TIME_SIG_WIDTH = 0.3f; // ?
                                          
      // Reduce available width
      if (numGlyphs > 2)
      {
        w = (m_width - TIME_SIG_WIDTH - 2 * xoff) / (numGlyphs - 2.0f);
      }
      xoff += TIME_SIG_WIDTH; // move other glyphs to the right      

      m_timeSigGlyph->x = x; // plus some extra?
      m_timeSigGlyph->y += y;
    }
  }

  // Set coord of each glyph
  // Compensate for glyph width, move to the left a bit
  // TODO depends on glyph type?, e.g. semibreve is slightly wider.
  float xfudge = -0.2f;

  for (auto& g : m_glyphs)
  {
    g->x = x + w * static_cast<float>(g->order) + xoff + xfudge;

    g->y += y; // TODO Whether or not this is correct will become
               //  apparent when we have multi-line scores.
  }

  // Set position of beam left and right ends
  for (auto& b : m_beams)
  {
    b->xmin = x + w * static_cast<float>(b->left) + xoff + xfudge;
    b->xmax = x + w * static_cast<float>(b->right) + xoff + xfudge;
    b->y += y;
  }
}
