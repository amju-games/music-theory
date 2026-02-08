// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2025
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
#include "Curve.h"
#include "Hairpin.h"
#include "KeySig.h"
#include "MakeScore.h"
#include "Pitch.h"
#include "Performance.h"
#include "Quad.h"
#include "RestGlyph.h"
#include "Suppress.h"
#include "TimeSig.h"
#include "TimeValue.h"
#include "Utils.h"

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

// ???
// This is called from Beam and Flag ... obviously, TODO
float GetHeight(BeamLevel bl)
{
  // Relies on the int values 0, 1...
  return static_cast<float>(bl);
}

Stave& MakeScore::GetCurrentStave()
{
  if (m_staves.empty())
  {
    AddStave();
  }
  return *(m_staves.back());
}

bool MakeScore::IsSlur(const std::string& s)
{
  if (s == "<slur>")
  {
    Curve* c = new Curve;
    m_lastSlur = c;
    c->SetScale(m_scale);

    // Attach to most reccent glyph if there is one
    GetCurrentStave().Attach(c, Attachment::LEFT);

    m_otherGlyphs.push_back(std::unique_ptr<IGlyph>(c));
  }
  else if (s == "</slur>")
  {
    GetCurrentStave().Attach(m_lastSlur, Attachment::RIGHT);
  }
  else
  {
    return false;
  }
  return true;
}

bool MakeScore::IsHairpin(const std::string& s)
{
  if (s == "<" || s == ">")
  {
    Hairpin* hp = new Hairpin;
    m_lastHairpin = hp;
    hp->SetScale(m_scale);
    hp->SetCrescendo(s == "<");

    // Attach to most reccent glyph if there is one
    GetCurrentStave().Attach(hp, Attachment::LEFT);

    m_otherGlyphs.push_back(std::unique_ptr<IGlyph>(hp));
  }
  else if (s == "/<" || s == "/>")
  {
    GetCurrentStave().Attach(m_lastHairpin, Attachment::RIGHT);
  }
  else
  {
    return false;
  }
  return true;
}

bool MakeScore::IsPerformance(const std::string& s)
{
  if (s == "<stacc>")
  {
    GetCurrentStave().SetPerformance(SW_STACCATO);
  }
  else if (s == "</stacc>")
  {
    GetCurrentStave().ClearPerformance(SW_STACCATO);
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

void MakeScore::PreprocessTokens(std::vector<std::string>& tokens)
{
  // Goal of this function:
  // Move anything other than pitch, rest, and time value tokens to the
  //  outside of chord markers.
  // Then when we process those other tokens, the chord will already 
  //  exist in the bar, so we can attach to it (for directions, slurs, ties, etc.)

  bool inChord = false; // true if within ( ) chord marker tokens

  // Make a new token list, to which we only append. I.e. we don't 
  //  move the tokens around in-place; we create a new token sequence.
  std::vector<std::string> newTokens;

  // List of tokens within chord markers which we save until the
  //  end of the chord, then append to the output.
  std::vector<std::string> moveToAfterChord;

  // This implementation avoids all the iterator problems of manipulating
  //  the sequence in-place. It's deliberately simple and hopefully easy
  //  to understand. 
  for (auto it = tokens.begin(); it != tokens.end(); ++it)
  {
    auto token = *it;
    Trim(token);

    if (IsChordStart(token))
    {
      inChord = true;
      newTokens.push_back(token);
    }
    else if (IsChordEnd(token))
    {
      inChord = false;
      newTokens.push_back(token);

      // Append saved tokens to new list, then clear saved tokens list.
      newTokens.insert(newTokens.end(), 
        moveToAfterChord.begin(), moveToAfterChord.end());
    
      moveToAfterChord.clear();
    } 
    else if (inChord)
    {
      // These are the token types which we want to stay within the chord.
      //  (not sure what to do with rests in chords tho)
      if (   IsImmediatePitch(token) 
          || IsDeferredPitch(token) 
          || IsImmediateTimeVal(token)
          || IsDeferredTimeVal(token)
          || IsImmediateRest(token) 
          || IsDeferredRest(token)) 
      {
        newTokens.push_back(token);
      }
      else 
      {
        // Every other token type, that is within a chord, we want to 
        //  move to the outside of the chord markers.
        // So, don't add this token to the new tokens yet.
        // Save token to add once we are outside of chord.
        moveToAfterChord.push_back(token);
      }
    }
    else
    {
      // Token is not chord related and we're not in chord
      newTokens.push_back(token);
    }
  }
  // Overwrite the old tokens with the new list.
  tokens = newTokens;
}

// Tokenise input string 
std::vector<std::string>  MakeScore::Tokenise()
{
  std::stringstream ss(m_input);

  // Split space-separated input into a vector of strings.
  std::vector<std::string> tokens 
  {
    std::istream_iterator<std::string>{ss},
    std::istream_iterator<std::string>{}
  };
  
  return tokens;
}

void MakeScore::ParseGlobalSettings(std::vector<std::string>& tokens)
{
}

void MakeScore::Parse(std::vector<std::string>& tokens)
{
  int n = tokens.size();

  bool isText = false;
  std::string text;

  Chord chord;
  bool inChord; // If true, we are within ( ) chord markers

  // Current time in piece, units are number of crotchets from start.
  // Set from duration of tokens, and explicitly by the 'time' token. 
  float crotchetTime = 0;

  for (int i = 0; i < n; i++)
  {
    std::string s = tokens[i]; // copy so we can strip quotes off etc

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
      continue;
    }

    if (s[0] == TEXT_QUOTE_OPEN)
    {
      // First token of some text
      isText = true;
      s = s.substr(1);
      if (s.back() == TEXT_QUOTE_CLOSE)
      {
        text += s.substr(0, s.size() - 1);
        AddText(text);
        isText = false;
        text.clear();
      }
      else
      {
        text = s;
      }
    }
    else if (s == "stave")
    { 
      AddStave();
    }
    else if (s == "|")
    {
      GetCurrentStave().AddBar(); // TODO different bar lines
    }
    else if (s == "t")
    {
      // Tie prev and next notes - we can get position of prev.
      GetCurrentStave().AddTie();
    }
    else if (IsPerformance(s))
    {
      // Nothing to do, if it's a switch, we flip a value
    }
    else if (IsSlur(s))
    {
      // Nothing to do
    }
    else if (IsHairpin(s))
    {
      // Nothing to do
    }
    else if (IsDirection(s)) // Wait, what are Performance and Direction
    {
      // If we're in a chord, we haven't added the notes to the bar yet,
      //  and attaching this dir to the most recent note in the input string
      //  won't work.
      AddDirection(s);
    }
    else if (IsBeam(s))
    {
      //// TODO /////GetCurrentStave().AddBeam(s);
    }
    else if (IsClef(s))
    {
      GetCurrentStave().AddClef(s);
    }
    else if (IsTimeSig(s))
    {
      GetCurrentStave().AddTimeSig(s);
    }
    else if (IsKeySig(s))
    {
      GetCurrentStave().AddKeySig(s);
    }
    else if (IsDeferredPitch(s))
    {
      m_lastPitch = GetPitch(s); // + m_transpose;
    }
    else if (IsImmediatePitch(s))
    {
      m_lastPitch = GetPitch(s); // + m_transpose;
      if (inChord)
      {
        chord.push_back({ m_lastPitch, m_lastTimeValToken });
      }
      else
      {
        crotchetTime = GetCurrentStave().AddNote(
          m_lastTimeValToken, m_lastPitch, crotchetTime);
      }
    }
    else if (IsChordStart(s))
    {
      inChord = true;
    }
    else if (IsChordEnd(s))
    {
      inChord = false;
      crotchetTime = GetCurrentStave().AddChord(chord, crotchetTime);
      chord.clear();
    }
    else if (IsImmediateRest(s))
    {
      crotchetTime += GetCurrentStave().AddRest(m_lastTimeValToken, crotchetTime);
    }
    // NB Deferred rests not supported for now, I don't think there's much point.
    else if (IsDeferredTimeVal(s))
    {
      // Store time val token for subsequent notes
      assert(s.size() > 2);
      // Strip off < >
      m_lastTimeValToken = s.substr(1, s.size() - 2);
    }
    else if (IsImmediateTimeVal(s))
    {
      m_lastTimeValToken = s;
      if (inChord)
      {
        chord.push_back({ m_lastPitch, m_lastTimeValToken });
      }
      else
      {
        crotchetTime = GetCurrentStave().AddNote(m_lastTimeValToken, m_lastPitch, crotchetTime);
      }
    }
    else if (s == "page-w")
    {
      i++;
      float pageWidth = atof(tokens[i].c_str());
      SetPageWidth(pageWidth);
      Stave::SetPageWidth(pageWidth);
    }
    else if (s == "time")
    {
      i++;
      crotchetTime = atof(tokens[i].c_str());
    }
    // FINAL ELSE
    else if (!isText)
    {
      std::cout << "// *ERROR* Unrecognised: " << s << "\n"; 
      // REPORT ERROR TODO
      // ? return;
    }
  }
}

void MakeScore::AddDirection(const std::string& s)
{
  Attachment* t = new Attachment;
  t->SetGlyphText(s);
  t->SetScale(m_scale);

  // Attach to most reccent glyph if there is one
  GetCurrentStave().Attach(t);

  // Use width to offset x. Set y - 0.5, which is below the stave.
  float w = 1.f;
  auto it = DIRECTIONS.find(s);
  if (it != DIRECTIONS.end())
  {
    w = it->second;
  }
  const float DIRECTION_WIDTH_MULT = 0.15f;
  w *= DIRECTION_WIDTH_MULT;
  // Offset x by half width so we centre the glyph horizontally
  t->SetPos(w * -0.5f, m_y);

  m_otherGlyphs.push_back(std::unique_ptr<IGlyph>(t));
}

void MakeScore::AddText(const std::string& s)
{
  Attachment* t = new Attachment;
  t->SetGlyphText("\"" + s + "\"");
  t->SetScale(m_scale);
  // Offset Y so text is above or below the current stave
  const float Y_ABOVE = 1.5f;
  t->SetPos(0, m_y + Y_ABOVE);

  // Attach to most reccent glyph if there is one
  GetCurrentStave().Attach(t);

  m_otherGlyphs.push_back(std::unique_ptr<IGlyph>(t));
}

void MakeScore::AddStave()
{
  float h = 0;
  if (!m_staves.empty())
  {
    h -= m_staves.back()->GetHeight();
  }
  auto stave = std::make_unique<Stave>();
  stave->SetScale(m_pageWidth, m_scale);
  stave->SetPos(0, h); 
  m_staves.emplace_back(std::move(stave));
}

void MakeScore::MakeInternal()
{
  // Tokenise, preprocess, deal with global settings in the input,
  //  so later parsing only deals with music notation, not page/style stuff.
  auto tokens = Tokenise();
  PreprocessTokens(tokens);
  ParseGlobalSettings(tokens); // TODO

  // Tokenise input string and add each token to internal representation.
  // At this stage the only tokens should be music notation, not settings.
  Parse(tokens);

  for (auto& stave : m_staves)
  {
    stave->CalcBarSizesAndPositions();
    stave->CalcStartTimes();
  }

  ToStringInternal();
}

std::string MakeScore::ToString()
{
  std::string res;
  for (const auto& s : m_outputLines)
  {
    res += s + "\n";
  }
  return res;
}

void MakeScore::ToStringInternal()
{
  std::string res;

  for (const auto& stave : m_staves)
  {
    res += stave->ToString();
  }

  res += BarLinesToString();

  for (const auto& g : m_otherGlyphs)
  {
    res += g->ToString();
    res += LineEnd(m_outputOnOneLine);
  }

  // Output every beat, with its normalized time.   
  // Only need to do it for one stave.
  if (!m_staves.empty())
  {
    res += m_staves.back()->OutputBeats(); // TODO turn off if not required
  }

  m_outputLines = Split(res, '\n');
}

std::string MakeScore::BarLinesToString()
{
  std::string res;
  if (m_staves.empty()) return "";

  const bool yesComments = (GetSuppressFlags() & META_COMMENT) == 0;

  // Draw quads through all staves. Use stave 0 for all bar widths and
  //  bar line types. (Vertically aligned bars on all staves should have
  //  the same width.)
  // +ve y is UP

  // TODO Same offset that we have to add to stems. 
  // Perhaps all quads are off by this in y, because...  glyphs have it
  //  added somewhere, perhaps???
  const float YOFF = 0.475f; // WHYYYYY

  // top of top stave
  float maxY = YOFF + m_staves.front()->GetY() + 4 * STAVE_LINE_GAP; 
  // bottom of bottom stave.
  float minY = YOFF + m_staves.back()->GetY(); 

  const int numBars = m_staves.front()->GetNumBars();
  for (int i = 0; i < numBars; i++)
  {
    const float x = m_staves.front()->GetBar(i).GetBarLineX();

    if (yesComments)
      res += "// Bar line " + std::to_string(i) + LineEnd(); 

    // Quad(float xmin, float ymin, float xmax, float ymax);
    Quad q(
      (x - BAR_LINE_WIDTH) * m_scale, 
       minY * m_scale,
       x * m_scale, 
       maxY * m_scale);
    res += q.ToString() + LineEnd();
  } 
  return res;
}

