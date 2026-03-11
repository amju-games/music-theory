// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <iostream>
#include "Bar.h"
#include "NoteGlyph.h"
#include "RestGlyph.h"
#include "Stem.h"
#include "Suppress.h"

bool IsBarLine(const std::string& s)
{
  return GetBarLine(s) != BarLine::BAR_LINE_NOT_SET;
}

BarLine GetBarLine(const std::string& s)
{
  if (s == "|") return BarLine::BAR_LINE_SINGLE;
  if (s == "||") return BarLine::BAR_LINE_DOUBLE;
  return BarLine::BAR_LINE_NOT_SET;
}

void Bar::CopyState(const Bar& b)
{
  SetScale(b.m_scale);
  // Copy time sig over to next bar
  SetTimeSig(b.GetTimeSig());
  // Copy key sig over
  SetKeySig(b.GetKeySig());
  // Copy clef over
  m_currentClef = b.m_currentClef;
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
// Easy if there's a time sig, it's just the number of beats in the bar.
// If no time sig, we try to calculate it:
// The time val of all the glyph members should add up to this.
TimeValue Bar::GetDuration() const
{
  return static_cast<float>(GetNumBeats()) * TIMEVAL_CROTCHET;
}

int Bar::GetNumBeats() const
{
  if (m_numBeats != -1) return m_numBeats;

  auto [ numBeats, timeMult ] = GetNumBeatsAndCrotchetValue(m_timeSig);
  
  if (numBeats == 0)
  {
    // No time sig, so add up duration of all sequential glyphs
    float d = 0;
    for (auto& g : m_glyphs)
    {
      // get duration, float, in crotchets
      TimeValue timeVal = g->GetTimes().GetTimeValue(); 
      d += timeVal;
    }
    numBeats = static_cast<int>(d + .5f);
    // Beef up a small number of beats so spacing algo works
    numBeats = std::max(2, numBeats);
  }
  m_numBeats = numBeats;

  return m_numBeats;
}

void Bar::CalcNormalisedTimes(const TimeValue totalPieceDuration)
{
  auto [ _, timeMult ] = GetNumBeatsAndCrotchetValue(m_timeSig);

  for (auto& g : m_glyphs)
  {
    // Calc scale factor to normalise times. timeMult adjusts for compound
    //  times, but basically we are dividing start times and durations by
    //  the total duration of the piece.
    float scale = timeMult / totalPieceDuration;
    g->NormaliseTimes(scale);
  }
}

void Bar::SetScale(float scale)
{
  m_scale = scale;
}

float Bar::AddRest(const std::string& token, int switches, float startTimeValue, bool isWholeBar)
{
  int order = static_cast<int>(m_glyphs.size());

  auto glyph = std::make_unique<RestGlyph>(token, isWholeBar, order);
  glyph->SetScale(m_scale);
  auto& times = glyph->GetTimes();
  times.Set(token);
  times.SetStartTime(startTimeValue);
  auto duration = times.GetTimeValue();
  m_glyphs.push_back(std::move(glyph));

  return startTimeValue + duration;
}

std::unique_ptr<ChordGlyph> Bar::CreateChordGlyph(
  const Chord& ch,
  int switches,
  int xOrder,
  float crotchetTime)
{
  auto chordGlyph = std::make_unique<ChordGlyph>(ch);
  chordGlyph->SetScale(m_scale);

  for (const auto& [pitch, duration] : ch)
  {
    auto noteGlyph = CreateNoteGlyph(duration, pitch, switches, xOrder, crotchetTime);
    chordGlyph->AddNoteGlyph(std::move(noteGlyph));
  }
  // Get the input token of the 0th note in the chord - which should be
  //  the longest duration (notes are sorted by duration)
  const auto durationToken = ch[0].second;
  chordGlyph->GetTimes().Set(durationToken);
  chordGlyph->GetTimes().SetStartTime(crotchetTime);

  chordGlyph->SetStem();

  return chordGlyph;
}

std::unique_ptr<NoteGlyph> Bar::CreateNoteGlyph(
  const std::string& durationToken,
  Pitch pitch,
  int switches,
  int xOrder,
  float startTimeValue)
{
  auto noteGlyph = std::make_unique<NoteGlyph>(durationToken, xOrder);
  noteGlyph->SetScale(m_scale);
  noteGlyph->SetPerformance(switches); // but can pause a rest
  noteGlyph->SetPitch(pitch);

  // Calc y, using current pitch, stave, and clef. 
  noteGlyph->CalcY(m_keySig, m_currentClef);

  // Calc any accidental required for the given pitch in the 
  //  current key. 
  // Handled when overriden by specifying step/octave/alter.
  noteGlyph->CalcAccidental(m_keySig);

  // Accidental 2nd pass: adjust based on previous accidental
  //  for the stave line for this note
  Accidental prev = m_accidentals[noteGlyph->GetStaveLine()];
  noteGlyph->AdjustAccidental(prev);

  // Store most recent acc for the stave line of this note
  m_accidentals[noteGlyph->GetStaveLine()] = noteGlyph->GetAccidental();

  // Set duration - calc the time val in crotchets, and save the
  //  raw token, for final render output and comment output.
  noteGlyph->GetTimes().Set(durationToken);

  // Set the start time value, units are crotchets, same as duration.
  noteGlyph->GetTimes().SetStartTime(startTimeValue);

  return noteGlyph;
}

float Bar::AddNote(const std::string& token, Pitch pitch, int switches,
  float startTimeValue)
{
  int order = static_cast<int>(m_glyphs.size());

  auto noteGlyph = 
    CreateNoteGlyph(token, pitch, switches, order, startTimeValue);

  noteGlyph->SetStem(); // outside of Create because chords also call that.
  auto duration = noteGlyph->GetTimes().GetTimeValue();
  m_glyphs.push_back(std::move(noteGlyph));

  return startTimeValue + duration;
}

float Bar::AddChord(
  const Chord& ch, int switches,
  float startTimeValue)
{
  assert(!ch.empty());

  int order = static_cast<int>(m_glyphs.size());

  auto chordGlyph = CreateChordGlyph(ch, switches, order, startTimeValue);
  auto duration = chordGlyph->GetTimes().GetTimeValue();
  m_glyphs.push_back(std::move(chordGlyph));

  return startTimeValue + duration;
}

void Bar::SetClef(Clef clef)
{
  // Has clef changed? If so, output a mini-clef at the end of the bar.
  // Not if this is the first bar though???? Not sure about this - TODO
  // I think we need to add the mini-clef to the PREVIOUS bar.
  if (    clef != m_currentClef
      && !m_isFirstBarOfLine) // ?
  {
    m_yesOutputMiniClef = true;
  }
  m_currentClef = clef;
}

void Bar::AddTimeSig(const std::string& s)
{
  SetTimeSig(::GetTimeSig(s));

  Glyph* glyph = new TimeSigGlyph(s);
  glyph->SetScale(m_scale);
  m_timeSigGlyph = std::unique_ptr<Glyph>(glyph);
}

void Bar::AddBeam(const std::string& s)
{
}

std::string Bar::ToString()
{
  std::string res;

  bool yesComments = (GetSuppressFlags() & META_COMMENT) == 0;

  // Clef for each stave, if first bar of line, and single or double
  //  stave - not if no stave or just rhythm line
  if (YesShowClefAtFrontOfBar())
  {
    float x = 0;
    float y = m_y; 

    res += GetClefOutputString(m_currentClef, x, y, m_scale) + 
      LineEnd();
  }

  // Key sig
  if (m_isFirstBarOfLine)
  {
    float x = 0;
    if (YesShowClefAtFrontOfBar()) // always true if showing key sig?
    {
      // offset to avoid clef, should be done by accumulating x pos
      x = CLEF_WIDTH;
    }
    float y = m_y;
    res += GetKeySigOutputString(m_keySig, m_currentClef, x, y, m_scale) + 
      LineEnd();
  }  

  // Optional time sig
  if (m_timeSigGlyph)
  {
    if (yesComments)
    {
      res += m_timeSigGlyph->CommentString() + LineEnd();
    }
    res += m_timeSigGlyph->ToString() + LineEnd();
  }

  for (auto& g : m_glyphs)
  {
    if (yesComments)
    {
      res += g->CommentString() + LineEnd();
    }
    res += g->ToString() + LineEnd();
  }

  for (auto& b : m_beams)
  {
    b->SetPos(m_x, m_y); // set pos of bar, to add to beam coords
    b->SetScale(m_scale);
    if (yesComments)
    {
      res += b->CommentString() + LineEnd();
    }
    res += b->ToString() + LineEnd();
  }

  return res;
}

bool Bar::YesShowClefAtFrontOfBar() const
{
  return (m_isFirstBarOfLine);
}

float Bar::GetRelativeWidth() const
{
  float w = static_cast<float>(GetNumBeats());
  if (YesShowClefAtFrontOfBar())
  {
    w += 1; // clef
    // TODO Key sig width
    //w += GetKeySigWidth() * 5.0f; // scale up key sig width so it's about right
  }
  if (m_timeSigGlyph)
  {
    w += 1;
  }
  return w;
}

void Bar::CalcWidth(float totalWidth, float pageWidth, float widthScale)
{
  float relW = GetRelativeWidth();

  if (m_scale == 0)
  {
    std::cout << "Div by zero! m_scale == 0!\n";
    m_scale = 1.f;
  }

  m_width = relW / totalWidth * pageWidth / m_scale;
  m_width *= widthScale;
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
  m_x = x; // Remember for bar lines
  m_y = y;

  // w is the width between glyphs
  float w = 0;

  // Reduce available bar width when we have time sig, key sig, clef.
  float reduction = 0;

  // Displaying clef at front of this bar?
  if (YesShowClefAtFrontOfBar())
  {
    // Need space for clef, so shunt everything right
    float clefW = CLEF_WIDTH;

    // Also we must be outputting key sig
    clefW += GetKeySigWidth();

    // Add a bit extra so there is a small space before the time sig
    const float EXTRA_SPACE = 0.1f;
    clefW += EXTRA_SPACE;

    reduction += clefW;
    x += clefW;
  }

  if (m_timeSigGlyph)
  {
    reduction += TIME_SIG_WIDTH;
    m_timeSigGlyph->SetPos(x, m_timeSigGlyph->GetY() + y);
    x += TIME_SIG_WIDTH;
  }

  // xoff is distance from left edge to first glyph, and also distance
  //  from last glyph to right bar line.
  // 'Edge' is the left bar line, OR right side of clef, keysig, timesig,
  //   whichever is most to the right.
  int numBeats = GetNumBeats();
  float xoff = (m_width - reduction) / (numBeats + 1.0f);

  // Reduce total width, and divide this by the number of beats to get 
  //  the distance between each beat.
  // x-coord is time (in beats - bar start time in beats) * beat width.
  // TODO Should we use timeMult here?
  if (numBeats > 1)
  {
    w = (m_width - reduction - 2 * xoff) / (numBeats - 1.0f);
  }
  else
  {
    w = (m_width - reduction - 2 * xoff);
  }

  // OK to get time val of 0th glyph as start time of bar?
  TimeValue barStartTime  = 0;
  if (!m_glyphs.empty())
  { 
    barStartTime = m_glyphs.front()->GetTimes().GetStartTimeValue();
  }
  // Set coord of each glyph
  // Compensate for glyph width, move to the left a bit
  // TODO depends on glyph type?, e.g. semibreve is slightly wider.
  float xfudge = -0.2f;

  for (auto& g : m_glyphs)
  {
    // Bar coord is (x, y), we offset by the position we calculate in x,
    //  and by the glyph's own y-offset in y.
    TimeValue glyphTimeInBar = g->GetTimes().GetStartTimeValue() - 
      barStartTime;
    float xPosInBar = w * glyphTimeInBar + xoff + xfudge;
    g->SetPos(
      xPosInBar + x,  // position within bar + pos of bar in x
      g->GetY() + y); // y-coord of glyph + pos of bar in y
  }
}

float Bar::GetBarLineX() const
{
  return m_x + m_width;
}

void Bar::MakeBeamGroups()
{
  // Find beam groups in this bar. Pass in time sig so we can split
  //  beams on major beats.
  auto beamGroups = FindBeamGroups(m_glyphs, m_startTime, m_timeSig);

  for (auto& bg : beamGroups)
  {
    bg.DecideStemDirections(m_glyphs); // beam goes above or below?
    bg.CalcYStaveLinesAtEnds(m_glyphs); // and set stem lengths here too

    // Add beams for rendering in ToString
    bg.AddBeams(m_beams, m_glyphs);
  } 
}

