// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <iostream>
#include "Bar.h"
#include "LayoutStrategy.h"
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

// Just a basic impl of Strategy pattern
static std::unique_ptr<LayoutStrategy> s_layoutStrategy;

void Bar::SetLayoutStrategy(LayoutStrategy* ls)
{
  s_layoutStrategy.reset(ls);
}

LayoutStrategy* Bar::GetLayoutStrategy()
{
  assert(s_layoutStrategy);
  return s_layoutStrategy.get();
}

void Bar::CopyState(const Bar& b)
{
  // Copy part of the given bar, but not everything. E.g. we need to know
  //  the clef, but we don't want the 'clef changed' flag because that could
  //  be different from the preceding bar.

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
  // TODO If setting rather than copying from preceding bar, that means
  //  a key sig change, riiight? There's no AddKeySig....
}

KeySig Bar::GetKeySig() const
{
  return m_keySig;
}

void Bar::SetTimeSig(TimeSig ts)
{
  m_timeSig = ts;
  //  That's it -- see AddTimeSig().
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
  const auto [ _, timeMult ] = GetNumBeatsAndCrotchetValue(m_timeSig);

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

float Bar::AddRest(const std::string& token, 
  [[maybe_unused]]int switches, float startTimeValue, bool isWholeBar)
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

  // Create a note glyph for each note in the chord
  for (const auto& [pitch, duration] : ch)
  {
    auto noteGlyph = CreateNoteGlyph(
      duration, pitch, switches, xOrder, crotchetTime);
    chordGlyph->AddNoteGlyph(std::move(noteGlyph));
  }
  // Get the duration (input) token of the 0th note in the chord - which should be
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
  // TODO Courtesy/cautionary clefs (the mini ones).

  m_currentClef = clef;
  // Set flag so we know to output clef at the start of this bar;
  // (If this is set, OR it's the first bar on a line, output the current clef)
  m_yesOutputPreNoteZoneClef = true; 
}

void Bar::AddTimeSig(const std::string& s)
{
  SetTimeSig(::GetTimeSig(s));

  Glyph* glyph = new TimeSigGlyph(s);
  glyph->SetScale(m_scale);
  m_timeSigGlyph = std::unique_ptr<Glyph>(glyph);

  // Set flag so we know to output timesig at the start of this bar?
  // No, because we can use the non-nullness of m_timeSigGlyph. We don't add the
  //  prevailing time sig at the start of every line.
}

std::string Bar::BarNumberString(int barNum) const
{
  // Output bar number and position of this bar
  return "BAR_NUMBER, " + std::to_string(barNum) + ", " + 
    std::to_string(m_x * m_scale) + ", " + 
    std::to_string(m_y * m_scale) + 
    LineEnd(); 
}

bool Bar::YesShowPreNoteZoneKeySig() const
{
  return m_isFirstBarOfLine || m_yesOutputPreNoteZoneKeySig;
}

float Bar::GetNoteZoneLeftX() const
{
  return GetX() + m_preNoteZoneWidth;
}

float Bar::GetNoteZoneWidth() const
{
  return m_width - m_preNoteZoneWidth - m_postNoteZoneWidth;
}

float Bar::CalcPreNoteZoneWidth() const
{
  float x = 0;

  if (YesShowPreNoteZoneClef()) { x += CLEF_WIDTH; }

  // TODO Neutraliser width
  if (YesShowPreNoteZoneKeySig()) { x += GetKeySigWidth(); }  

  if (m_timeSigGlyph) { x += TIME_SIG_WIDTH; }

  return x;
}

std::string Bar::PreNoteZoneToString()
{
  // Pre-note zone elements:
  // 0. Double barline if key or time sig changes, and not start of line
  // 1. Clef: if clef change or start of line
  // 2. Key sig: if start of line or key sig change. We first cancel any
  //    sharps or flats in the old key sig that are naturals in the new 
  //    key sig. So the key sig is comprised of two parts: "neutraliser" and
  //    new key.
  // 3. Time sig: if first bar or key sig change; ("mini bar" with time sig
  //    at end of line if the time sig changes in the next bar on a 
  //    new line).
  // 4. Courtesy clef
  // Clef for each stave, if first bar of line, and single or double
  //  stave - not if no stave; percussion clef for rhythm line - that's TODO

  const bool yesComments = (GetSuppressFlags() & MD_COMMENT) == 0;
  float x = 0; // x-coord within bar; also the width of the pre-note zone.
  float y = m_y; 

  std::string res;

  // TODO double bar line here

  if (YesShowPreNoteZoneClef())
  {
    res += GetClefOutputString(m_currentClef, x, y, m_scale) + 
      LineEnd();
    x += CLEF_WIDTH;
  }

  // Key sig
  if (YesShowPreNoteZoneKeySig()) 
  {
    // TODO Neutraliser here

    res += GetKeySigOutputString(m_keySig, m_currentClef, x, y, m_scale) + 
      LineEnd();
    x += GetKeySigWidth();
  }  

  // Time sig
  if (m_timeSigGlyph)
  {
    // TODO Create a "mini bar" containing only the new time sig,  if this 
    //  bar is at end of line.

    // Set the pos of the time sig: TODO pass these into ToString for consistency.
    m_timeSigGlyph->SetPos(x, y);

    if (yesComments)
    {
      res += m_timeSigGlyph->CommentString() + LineEnd();
    }
    res += m_timeSigGlyph->ToString() + LineEnd();
    x += TIME_SIG_WIDTH;
  }

  return res;
}

void Bar::SetPreNoteZoneWidth(float w)
{
  // Overwrite width calculated in GeneratePreNoteZone so all vertically
  //  aligned zones have the same width.
  m_preNoteZoneWidth = w;
}

void Bar::SetPostNoteZoneWidth(float w)
{
  m_postNoteZoneWidth = w;
}

std::string Bar::ToString()
{
  std::string res;

  const bool yesComments = (GetSuppressFlags() & MD_COMMENT) == 0;

  // Each bar splits into two logical 'zones': pre-note zone, and note zone.
  // The layout strategy controls glyph placement in the note zone, but the 
  //  pre-note zone follows the same rules all the time and is fixed.
  // If we are drawing multiple lines/systems, there will be a post-note zone too.
  // For single-line, (i.e. Piano Fest game), we don't need a post-note zone.
  res += PreNoteZoneToString();

  // Output the notes and rests in the note zone: this is just a simple loop - 
  //  the more complicated bit was setting their x-coords.
  for (auto& g : m_glyphs)
  {
    if (yesComments)
    {
      res += g->CommentString() + LineEnd();
    }
    res += g->ToString() + LineEnd();
  }

  // Output beams.
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

bool Bar::YesShowPreNoteZoneClef() const
{
  return (m_yesOutputPreNoteZoneClef || m_isFirstBarOfLine);
}

float Bar::GetRelativeWidth() const
{
  // Get the width of this bar relative to other bars. 
  // We delegate the note zone portion of this to the layout strategy,
  //   because the width of the note zone depends on how we space out notes. 
  return 
    m_preNoteZoneWidth + 
    GetLayoutStrategy()->CalcNoteZoneWidth(*this) +
    m_postNoteZoneWidth;
}

void Bar::CalcWidth(float totalWidth, float pageWidth, float widthScale)
{
  // NOT delegated to the layout strategy: this just scales bar widths.

  if (m_scale == 0)
  {
    // TODO Report Error
    std::cout << "// Div by zero! m_scale == 0!\n";
    m_scale = 1.f;
  }

  // This bar gets a fraction of the page width, scaled by bar-specific
  //  widthScale, and m_scale which is the overall scale factor for all bars,
  //  and is applied to all glyphs.

  //  TODO  (m_scale should be static, no? Hmm not for
  //  e.g. one stave  has smaller glyphs. Should be passed from stave?)

  const float fractionOfPageForThisBar = widthScale / totalWidth;

  // TODO I'm not sure about m_scale here. Scale affects glyph sizes, not
  //  bar widths, no?
  m_width = pageWidth * fractionOfPageForThisBar / m_scale;
}

float Bar::GetWidth() const
{
  return m_width;
}

float Bar::GetKeySigWidth() const
{
  // KeySig is just an enum so no member funcs

  const float ACC_WIDTH = NOTE_HEAD_WIDTH * .8f;
  if (m_keySig >= KEYSIG_0_FLAT)
  {
    // Flat
    return (m_keySig - KEYSIG_0_FLAT) * ACC_WIDTH;
  }
  else
  {
    // Sharp
    return (m_keySig - KEYSIG_0_SHARP) * ACC_WIDTH;
  }
}

void Bar::SetPos(float x, float y)
{
  m_x = x; 
  m_y = y;
}

void Bar::PositionGlyphs()
{
  // Calc the start of the Note Zone and its width.
  float noteZoneX = m_x + m_preNoteZoneWidth;
  float noteZoneWidth = m_width - m_preNoteZoneWidth - m_postNoteZoneWidth;

  // TODO Ask the Layout Strategy if we want to centre this single glyph.
  // This is really just about whole bar rests but could apply to other things?
  if (   m_glyphs.size() == 1 
      && m_glyphs[0]->ShouldCentreIfSingle())
  {
    CentreSingleGlyph(noteZoneX, noteZoneWidth);
    return;
  }

  // Delegate positioning notes to the Layout Strategy.
  GetLayoutStrategy()->PositionGlyphs(*this);
}

void Bar::CentreSingleGlyph(float leftX, float noteZoneWidth)
{
  // This isn't part of the Layout Strategy, because it's just finding the
  //  centre of the note zone. But the decision of whether or not to do this
  //  should be made by the Strategy.

  auto& g = m_glyphs[0];
  float xCentre = noteZoneWidth / 2.f;
  g->SetPos(xCentre + leftX, g->GetY() + m_y); 
}

float Bar::GetBarLineX() const
{
  return m_x + m_width;
}

void Bar::MakeBeamGroups()
{
  // Find beam groups in this bar. 
  // Pass in time sig so we can split beams on major beats.
  auto beamGroups = FindBeamGroups(m_glyphs, m_startTime, m_timeSig);

  for (auto& bg : beamGroups)
  {
    bg.DecideStemDirections(m_glyphs); // beam goes above or below?
    bg.CalcYStaveLinesAtEnds(m_glyphs); // and set stem lengths here too

    // Add beams for rendering later, in ToString()
    bg.AddBeams(m_beams, m_glyphs);
  } 
}

