// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017
//
// * MakeScore *
//
// Sub project to convert easily-authorable music content
//  into list of glyphs.


// Music score coords:
//  +--------------------------+   0.4
//  +--------------------------+
//  +--------------------------+
//  +--------------------------+
//  +--------------------------+   0.0
// 0.0                     DEFAULT_PAGE_WIDTH

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "Clef.h"
#include "Consts.h"
#include "KeySig.h"
#include "MakeScore.h"
#include "Pitch.h"
#include "Switch.h"
#include "TimeSig.h"
#include "TimeValue.h"
#include "Utils.h"

// Current stave
static int s_stave = 0;

// Bit field for staccato, accent, pause, etc., per stave
static int s_switches[MAX_NUM_STAVES] = { 0, 0, 0, 0 };

// Const set of performance directions, with relative widths
static const std::map<std::string, float> DIRECTIONS = 
{
  { "f",  1.f },
  { "p",  1.f },
  { "ff", 2.f },
  { "pp", 2.f },
  { "mp", 2.5f },
  { "mf", 2.5f }
};

bool IsDirection(const std::string& s)
{
  return DIRECTIONS.find(s) != DIRECTIONS.end();
}

float GetHeight(BeamLevel bl)
{
  // Relies on the int values 0, 1...
  return static_cast<float>(bl);
}

std::string LineEnd(bool oneLine)
{
  return (oneLine ? ";" : "\n");
}

bool IsSwitch(const std::string& s)
{
  if (s == "<stacc>")
  {
    SetSwitch(s_switches[s_stave], SW_STACCATO);
  }
  else if (s == "</stacc>")
  {
    ClearSwitch(s_switches[s_stave], SW_STACCATO);
  }
  else
  {
    // None of the above: not a switch
    return false;
  }
  return true;
}

void MakeScore::Preprocess()
{
  //  std::cout << "Preprocessed input: " << m_input << "\n";
}

// Tokenise input string and add each token to internal representation.
void MakeScore::AddTokens()
{
  std::stringstream ss(m_input);

  // Split space-separated input into a vector of strings.
  std::vector<std::string> strs{
    std::istream_iterator<std::string>{ss},
    std::istream_iterator<std::string>{}
  };

  int n = strs.size();

  bool isText = false;
  std::string text;

  for (int i = 0; i < n; i++)
  {
    std::string s = strs[i]; // copy so we can strip quotes off

    if (s == "|")
    {
      Bar* bar = new Bar;
      bar->CopyState(*m_bars.back());
      m_bars.push_back(std::unique_ptr<Bar>(bar));
    }
    else if (s == "t")
    {
      // Tie prev and next notes - we can get position of prev.
      AddTie();
    }
    else if (IsSwitch(s))
    {
      // Nothing to do, if it's a switch, we flip a value
    }
    else if (IsDirection(s))
    {
      AddDirection(s);
    }
    else if (IsBeam(s))
    {
      AddBeam(s);
    }
    else if (IsClef(s))
    {
      AddClef(s);
    }
    else if (IsTimeSig(s))
    {
      AddTimeSig(s);
    }
    else if (IsKeySig(s))
    {
      AddKeySig(s);
    }
    else if (IsDeferredPitch(s))
    {
      m_lastPitch = GetPitch(s) + m_transpose;
    }
    else if (IsImmediatePitch(s))
    {
      m_lastPitch = GetPitch(s) + m_transpose;
      AddGlyph();
    }
    else if (IsDeferredTimeVal(s))
    {
      // Store time val token for subsequent notes
      assert(s.size() > 2);
      m_lastTimeValToken = s.substr(1, s.size() - 2);
    }
    else if (IsImmediateTimeVal(s))
    {
      m_lastTimeValToken = s;
      AddGlyph();
    }
    else if (s[0] == TEXT_QUOTE_OPEN)
    {
      // First token of some text
      // text = s.substr(1);
      isText = true;
      s = s.substr(1);
    }
    else if (!isText)
    {
      std::cout << "// *ERROR* Unrecognised: " << s << "\n"; 
      // REPORT ERROR TODO
      // ? return;
    }

    // Text - add to string and check for final quote
    if (isText)
    {
      if (!text.empty())
      {
        text += " ";
      }
      if (s.back() == TEXT_QUOTE_CLOSE)
      {
        text += s.substr(0, s.size() - 1);
        AddText(text);
        isText = false;
        text.clear();
      }
      else
      {
        text += s;
      }
    }
  }
}

void MakeScore::Attach(Attachment* t, int leftOrRight)
{
  auto& b = m_bars.back();
  auto& g = b->GetGlyphs();
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

void MakeScore::AddDirection(const std::string& s)
{
  Attachment* t = new Attachment;
  t->SetGlyphText(s);
  t->SetScale(m_scale);

  // Attach to most reccent glyph if there is one
  Attach(t);

  // Use width to offset x. Set y = 0, which is below the stave.
  float w = 1.f;
  auto it = DIRECTIONS.find(s);
  if (it != DIRECTIONS.end())
  {
    w = it->second;
  }
  const float DIRECTION_WIDTH_MULT = 0.15f;
  w *= DIRECTION_WIDTH_MULT;
  // Offset x by half width so we centre the glyph horizontally
  t->SetPos(w * -0.5f, 0);

  m_otherGlyphs.push_back(std::unique_ptr<IGlyph>(t));
}

void MakeScore::AddText(const std::string& s)
{
  Attachment* t = new Attachment;
  t->SetGlyphText("\"" + s + "\"");
  t->SetScale(m_scale);
  // Offset Y so text is above or below the current stave
  const float Y_ABOVE = 2.0f;
  t->SetPos(0, Y_ABOVE);

  // Attach to most reccent glyph if there is one
  Attach(t);

  m_otherGlyphs.push_back(std::unique_ptr<IGlyph>(t));
}

void MakeScore::AddKeySig(const std::string& s)
{
  KeySig ks = GetKeySig(s);
  ks = TransposeKeySig(ks, m_transpose);
  m_bars.back()->SetKeySig(ks);
}

void MakeScore::AddClef(const std::string& s)
{
  // NB Bar already has Current stave number
  
  m_bars.back()->SetClef(GetClef(s));
}

void MakeScore::AddGlyph()
{
  m_bars.back()->AddGlyph(
    m_lastTimeValToken, m_lastPitch, s_switches[s_stave]);

  // If last tie has no right connection, connect it now to the
  //  glyph we just added.
  if (!m_ties.empty())
  {
    Tie* tie = m_ties.back().get();
    if (!tie->IsRhsSet())
    {
      assert(!m_bars.empty());
      tie->SetRightGlyph(m_bars.back()->GetGlyphs().back().get());
    }
  }
}

void MakeScore::AddTie()
{
  // Set bar and position of the left glyph of the tie
  if (m_bars.empty())
  {
    std::cout << "// *** Error, no left glyph for tie to refer to.\n";
    return;
  }

  Tie* tie = new Tie;
  tie->SetLeftGlyph(m_bars.back()->GetGlyphs().back().get());

  tie->SetScale(m_scale);

  m_ties.push_back(std::unique_ptr<Tie>(tie));
}

void MakeScore::MakeInternal()
{
  // Add first default bar
  Bar* bar = new Bar;
  bar->SetIsFirstBarOfLine(true); // first bar
  bar->SetStaveType(m_staveType);
  bar->SetScale(m_scale);
  m_bars.push_back(std::unique_ptr<Bar>(bar));

  // Tokenise input string and add each token to internal representation.
  AddTokens();

  CalcBarSizesAndPositions();

  CalcStartTimes();
}

void MakeScore::CalcStartTimes()
{
  // First, get the total time duration for all bars.
  // Then normalise, and acculumate time values of all glyphs to set the
  //  starting time of each one.

  float totalDuration = 0;
  for (auto& bar : m_bars)
  {
    totalDuration += bar->GetDuration();
  }

  // Use totalDuration to normalise duration of each glyph, and cumulative
  //  duration, so we set the start time of each glyph within 0..1
 
  float acc = 0; 
  for (auto& bar : m_bars)
  {
    acc = bar->CalcNormalisedTimes(totalDuration, acc);  
  }
}

void MakeScore::CalcBarSizesAndPositions()
{
  // Now loop over the bars. From the number of glyphs in each bar,
  //  work out the relative width of each bar.
  // For now, assume only one line.
  int totalNumGlyphs = 0;
  for (auto& bar : m_bars)
  {
    float w = bar->GetRelativeWidth();
    totalNumGlyphs += static_cast<int>(w); // TODO use floats throughout
  }

  // Bar calculates its width as fraction of s_pageWidth 
  for (auto& bar : m_bars)
  {
    bar->CalcWidth(totalNumGlyphs, m_pageWidth);
  }

  // Set (left, bottom) position of each bar
  float x = 0;
  float y = m_y;

  for (auto& bar : m_bars)
  {
    bar->SetPos(x, y);
    x += bar->GetWidth();
  }

  // Set left and right positions of ties
  for (auto& tie : m_ties) 
  {
    // Look up positions of glyphs the tie connects
    tie->CalcPos(); 
  }
}

std::string MakeScore::ToString()
{
  std::string res;

  // First, output a stave. For rhythm only, it's a single line.
  // TODO Multiple lines 

  res += GetStaveString(m_staveType, 0, m_y, m_pageWidth, m_scale);
  res += LineEnd(m_outputOnOneLine);

  for (auto& b : m_bars)
  {
    res += b->ToString(m_outputOnOneLine);
  }

  for (auto& t : m_ties)
  {
    res += t->ToString();
    res += LineEnd(m_outputOnOneLine);
  }

  for (auto& g : m_otherGlyphs)
  {
    res += g->ToString();
    res += LineEnd(m_outputOnOneLine);
  }

  return res;
}

