// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <iostream>
#include "Attachment.h"
#include "Consts.h"
#include "Stave.h"
#include "Suppress.h"
#include "Tie.h"
#include "Utils.h"

float Stave::s_pageWidth = DEFAULT_PAGE_WIDTH;

void Stave::SetPageWidth(float pageWidth)
{
  s_pageWidth = pageWidth;
}

float Stave::GetPageWidth()
{
  return s_pageWidth;
}

float Stave::GetHeight() const
{
  // TODO TEMP TEST
  return 1.5f;
}

std::string Stave::CommentString() const
{
  return "// Stave, type: " + LineEnd(); // TODO type
}

std::string Stave::ToString() const
{
  const bool yesComments = (GetSuppressFlags() & META_COMMENT) == 0;

  std::string res;

  switch (m_type)
  {
  case StaveType::STAVE_TYPE_NONE:
    break;

  case StaveType::STAVE_TYPE_PERCUSSION:
    // TODO Percussion stave line for this and for notes
    res += "stave-line, " + Str(x) + ", " + Str(y) + 
      AddScaleStringIfRequired() + LineEnd();
    break;

  case StaveType::STAVE_TYPE_STANDARD:
    res += "stave, " + CoordString() + AddScaleStringIfRequired() + LineEnd();
    break;
  }

  int barNum = 0;
  for (const auto& b : m_bars)
  {
    if (yesComments)
      res += "// ** Bar " + std::to_string(barNum++) + " **" + LineEnd();

    res += b->ToString();
  }

  for (const auto& t : m_ties)
  { 
    res += t->ToString();
    res += LineEnd();
  }   

  return res;
}

Bar& Stave::GetCurrentBar()
{
  if (m_bars.empty())
  {
    AddFirstBar();
  }

  if (m_bars.back()->GetBarLine() != BarLine::BAR_LINE_NOT_SET)
  {
    // Last bar has got a bar line at the end, so we should add a new bar.
    AddBar();
  }

  return *(m_bars.back());
}

void Stave::AddBar()
{
  Bar* bar = new Bar;
  bar->CopyState(*m_bars.back());
  m_bars.push_back(std::unique_ptr<Bar>(bar));
}

void Stave::AddFirstBar()
{
  Bar* bar = new Bar;
  bar->SetIsFirstBarOfLine(true); 
  bar->SetScale(scaleY);
  m_bars.push_back(std::unique_ptr<Bar>(bar));
}

int Stave::GetNumBars() const
{
  return static_cast<int>(m_bars.size());
}

const Bar& Stave::GetBar(int i) const
{
  return *(m_bars[i]);
}

int Stave::GetTotalNumBeats() const
{
  int totalNumBeats = 0;
  for (const auto& b : m_bars)
  {
    totalNumBeats += b->GetNumBeats();
  }
  return totalNumBeats;
}

std::string Stave::OutputBeats() const
{
  // Output every beat in each bar with its time marker
  // (TODO I think this might be overkill for what we need)

  int totalNumBeats = GetTotalNumBeats();
  
  // Calc duration of one beat
  const float d = 1.f / static_cast<float>(totalNumBeats);
  float t = 0;
  // Output the beats
  std::string res;
  // Output beats with bar (one based) and beat number (one based)
  int bar = 1;
  for (const auto& b : m_bars)
  {
    auto [ numBeats, _ ] = GetNumBeatsAndCrotchetValue(b->GetTimeSig());
    for (int i = 1; i <= numBeats; i++)
    {   
      res += "TIME, " + Str(t) + LineEnd(); // one-value TIME variant
      t += d;
      res += "BEAT, " + std::to_string(bar) + ", " + std::to_string(i) + LineEnd();
    }   
    bar++;
  }
  return res;
}

void Stave::CalcBarSizesAndPositions()
{
  // Loop over the bars. From the number of glyphs in each bar,
  //  work out the relative width of each bar.
  // For now, assume only one line.
  float totalWidth = 0;
  for (auto& bar : m_bars)
  {
    float w = bar->GetRelativeWidth();
    totalWidth += w;
  }

  // Bar calculates its width as fraction of s_pageWidth 
  for (auto& bar : m_bars)
  {
    bar->CalcWidth(totalWidth, GetPageWidth());
  }

  // Set (left, bottom) position of each bar
  float barX = 0;
  float barY = y;

  for (auto& bar : m_bars)
  {
    bar->SetPos(barX, barY);
    barX += bar->GetWidth();
  }

  // Set left and right positions of ties
  for (auto& tie : m_ties)
  {
    // Look up positions of glyphs the tie connects
    tie->CalcPos();
  }
}

void Stave::CalcStartTimes()
{
  // First, get the total time duration for all bars.
  // Then normalise, and acculumate time values of all glyphs to set the
  //  starting time of each one.

  TimeValue totalDuration = 0; // units are crotchets
  for (auto& bar : m_bars)
  {
    totalDuration += bar->GetDuration();
  }

  // Use totalDuration to normalise start time and duration of each glyph
  for (auto& bar : m_bars)
  {
    bar->CalcNormalisedTimes(totalDuration);
  }
}

void Stave::AddTie()
{
  // Set bar and position of the left glyph of the tie
  if (m_bars.empty())
  {
    std::cout << "// *** Error, no left glyph for tie to refer to.\n";
    return;
  }

  Tie* tie = new Tie;
  tie->SetLeftGlyph(GetCurrentBar().GetGlyphs().back().get());

  tie->SetScale(scaleY);

  m_ties.push_back(std::unique_ptr<Tie>(tie));
}

float Stave::AddRest(const std::string& s, float crotchetTime)
{
  return GetCurrentBar().AddRest(s, m_switches, crotchetTime);
}

float Stave::AddChord(const Chord& chord, float crotchetTime)
{
  float newCrotchetTime = GetCurrentBar().AddChord(
    chord, m_switches,
    crotchetTime);

  // TODO Tied chords!

  return newCrotchetTime;
}

float Stave::AddNote(const std::string& timeToken, const Pitch& pitch, float crotchetTime)
{
  float newCrotchetTime = GetCurrentBar().AddNote(
    timeToken, pitch, m_switches,
    crotchetTime);

  // If last tie has no right connection, connect it now to the
  //  glyph we just added.
  if (!m_ties.empty())
  {
    Tie* tie = m_ties.back().get();
    if (!tie->IsRhsSet())
    {
      assert(!m_bars.empty());
      tie->SetRightGlyph(GetCurrentBar().GetGlyphs().back().get());
    }
  }

  return newCrotchetTime;
}

void Stave::AddKeySig(const std::string& s)
{
  KeySig ks = GetKeySig(s);
  ks = TransposeKeySig(ks, m_transpose);
  GetCurrentBar().SetKeySig(ks);
}

void Stave::AddClef(const std::string& s)
{
  GetCurrentBar().SetClef(GetClef(s));
}

void Stave::AddTimeSig(const std::string& s)
{
  GetCurrentBar().AddTimeSig(s);
}

void Stave::Attach(Attachment* t, int leftOrRight)
{
  auto& b = GetCurrentBar();
  auto& g = b.GetGlyphs();
  if (g.empty())
  {
    // Try prev bar
    if (m_bars.size() > 1)
    {   
      auto& b = m_bars[m_bars.size() - 2]; 
      auto& g = b->GetGlyphs();
      t->SetParent(g.back().get(), leftOrRight);
    }   
  }
  else
  {
    t->SetParent(g.back().get(), leftOrRight);
  }
}

