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
#include <fstream>
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
#include "LayoutFull.h"
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
  { "fff", 3.f },
  { "ff",  2.f },
  { "f",   1.f },
  { "mf",  2.5f },
  { "mp",  2.5f },
  { "p",   1.f },
  { "pp",  2.f },
  { "ppp", 3.f },
};

static bool IsDirection(const std::string& s)
{
  return DIRECTIONS.find(s) != DIRECTIONS.end();
}

void MakeScore::SetLayoutStrategy()
{
  Bar::SetLayoutStrategy(new LayoutFull);
}

bool MakeScore::Load(const std::string& filename)
{
  std::ifstream inputFile(filename);
  if (!inputFile.good())
  {
    std::cout << "Can't open file `" << filename << "`\n";
    return false;
  }
  std::string line;
  while (std::getline(inputFile, line))
  {
    if (line.size() > 1 && line.substr(0, 2) == "//")
      continue;

    m_input += line;
    m_input += " "; 
  }
  std::cout << "// Final input as one line:\n// " << m_input << "\n";
  return true;
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
    hp->SetPos(0, GetYForDirection());
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

void MakeScore::ParseGlobalSettings([[maybe_unused]] std::vector<std::string>& tokens)
{
}

void MakeScore::Parse(std::vector<std::string>& tokens)
{
  auto n = tokens.size();

  bool isText = false;
  std::string text;

  Chord chord;
  bool inChord = false; // If true, we are within ( ) chord markers

  // Current time in piece, units are number of crotchets from start.
  // Set from duration of tokens, and explicitly by the 'time' token. 
  float crotchetTime = 0;

  for (auto i = 0; i < n; i++)
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
      crotchetTime = 0; // reset time for the new stave!
    }
    else if (IsBarLine(s))
    {
      GetCurrentStave().GetCurrentBar().SetBarLine(GetBarLine(s));
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
      //  won't work. -- Fixed by preprocessing the tokens.
      AddDirection(s);
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
      bool isWholeBar = IsWholeBarRest(s);
      crotchetTime = GetCurrentStave().AddRest(m_lastTimeValToken, crotchetTime, isWholeBar);
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
      float pageWidth = static_cast<float>(atof(tokens[i].c_str()));
      SetPageWidth(pageWidth);
      Stave::SetPageWidth(pageWidth);
    }
    else if (s == "time")
    {
      i++;
      crotchetTime = static_cast<float>(atof(tokens[i].c_str()));
    }
    else if (s == "bpm")
    {
      // If set in input, we will output bpm tempo.
      i++;
      m_bpm = static_cast<float>(atof(tokens[i].c_str()));
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

float MakeScore::GetYForDirection() const
{
  // Set y-coord so direction is under notes etc on stave 0, and above
  //  notes etc on stave 1. This is ok so long as the only use case
  //  for this is 2-stave piano music; will need rethinking for more cases.
  int stave = static_cast<int>(m_staves.size()) - 1;
  stave = std::max(stave, 0);
  // +ve y is UP
  float y = -.5f;
  if (stave == 1) y = -.7f;
  return y + m_y; // don't worry about scale yet, right?
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
  t->SetPos(w * -0.5f, GetYForDirection()); // don't worry about scale yet, right?

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

bool MakeScore::CheckStaveLengthsAreEqual() const
{
  // Sanity check:
  // We expect and require numBars to be the same across all staves.

  if (m_staves.empty()) return true; 

  const int numBars = m_staves.front()->GetNumBars();
  for (auto& stave : m_staves)
  {
    if (stave->GetNumBars() != numBars)
    {
      return false;
    }
  } 
  return true;
}

void MakeScore::AdjustBarWidths()
{
  if (m_staves.empty()) return;

  const int numBars = m_staves.front()->GetNumBars();
  // Initialise bar widths -- we start with 1.0 for every bar.
  m_barWidths = std::vector<float>(numBars, 1.f); // 'fill ctor'
  float totalW = 0;

  for (int i = 0; i < numBars; i++)
  {
    // This is a bit basic. We would really need to make verticals
    //  from the bar in each stave.
    for (auto& stave : m_staves)
    {
      // Calc bar width scale factor
      float w = stave->GetBar(i).GetRelativeWidth();
      // Get the max, so the width depends on the widest bar vertically.
      w = std::max(m_barWidths[i], w);
      m_barWidths[i] = w;
    }
    totalW += m_barWidths[i];
  }

  // Pass 2: normalise so the sum of widths == the number of bars.
  // So busy bars are wider, not-busy bars are narrower.
  // E.g.
  // 
  //           | sb | m m | c c c c | q q q q q q q q |
  // w:          1    2     4         8
  // totalW: 15
  // scale:  .2667
  // scaled w:  .2667 .5334 1.067     2.134    
  // Total of widths is still 4.0, i.e. the number of bars, but the
  //  individual widths are weighted depending on how busy the bar is.
  // So we can stick to the specified page width: each bar gets
  //  page_width / num_bars * m_barWidths[i].
 
  float scale = static_cast<float>(numBars) / totalW;
  for (float& w : m_barWidths)
  {
    w *= scale;
  }
}

void MakeScore::GeneratePreAndPostNoteZoneWidths()
{
  // Generate pre- and post-note zone for every bar. For vertically aligned bars,
  //  we should take the width of the widest zone, in case there
  //  are differences (e.g. a clef change in one stave).

  const int numBars = m_staves.front()->GetNumBars();

  for (int i = 0; i < numBars; i++)
  {
    float maxPreNoteZoneWidth = 0;  
    float maxPostNoteZoneWidth = 0;

    [[maybe_unused]] int nstave = 0;
    for (auto& stave : m_staves)
    {
      assert(stave->GetNumBars() == numBars); // We checked for this earlier

      const float preNoteWidth = stave->GetBar(i).CalcPreNoteZoneWidth();

#ifdef DEBUG_SHOW_PRE_NOTE_WIDTH
std::cout << "// Pre note zone width for bar " 
  << i 
  << " stave " << nstave++ 
  << ": " 
  << preNoteWidth << "\n";
#endif

      maxPreNoteZoneWidth = std::max(maxPreNoteZoneWidth, preNoteWidth);

      // TODO Post-note zone
    }
 
    // 2nd pass: set the widest value for the pre- and post-note zones
    for (auto& stave : m_staves)
    {
      auto& bar = stave->GetBar(i);
      bar.SetPreNoteZoneWidth(maxPreNoteZoneWidth);
      bar.SetPostNoteZoneWidth(maxPostNoteZoneWidth);
    } 
  }
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

  if (!CheckStaveLengthsAreEqual())  // TODO other sanity checks
  {
    // TODO Report error properly
    std::cout << "ERROR! Num bars different across staves!\n";
    return;
  }

  // The pre- and post-note zones in each bar contains clef/keysig/timesigs,
  //  in fixed positions. We generate these first.
  GeneratePreAndPostNoteZoneWidths();

  // Get width scale factor for each bar (we use the same max value for the
  //  vertically corresponding bars in all staves).
  // Fills m_barWidths, which are widths, weighted to sum to the number of bars.
  AdjustBarWidths();  // Internally uses Layout Strategy pattern

  for (auto& stave : m_staves)
  {
    // Calc start times of bars and elements in them; normalise times
    //  for animation meta data.
    stave->CalcStartTimes();

    // Calc bar sizes, and the position of each bar.
    // This uses the widths calcalated above so nothing to move to Strategy.
    stave->CalcBarSizesAndPositions(m_barWidths);

    // Within each bar, position elements in the note zone.
    stave->PositionGlyphs(); // Internally uses Layout Strategy 

    stave->MakeBeamGroups(); // Do this last so we have positions of notes!
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
  const bool yesComments = (GetSuppressFlags() & MD_COMMENT) == 0;

  std::string res;

  if (m_bpm > 0.f)
  {
    res += "BPM, " + std::to_string(m_bpm) + LineEnd();
  }

  int staveNum = 0;
  for (const auto& stave : m_staves)
  {
    if (yesComments)
      res += "// *** STAVE " + std::to_string(staveNum++) + "  ***" + LineEnd();

    res += stave->ToString();
  }
 
  if (yesComments) res += "// Bar lines" + LineEnd();
  res += BarLinesToString();

  for (const auto& g : m_otherGlyphs)
  {
    if (yesComments)
      res += g->CommentString() + LineEnd();

    res += g->ToString() + LineEnd();
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

  const bool yesComments = (GetSuppressFlags() & MD_COMMENT) == 0;

  // Draw quads through all staves. Use stave 0 for all bar widths and
  //  bar line types. (Vertically aligned bars on all staves should have
  //  the same width.)
  // +ve y is UP

  // We have to add QUAD_Y_OFFSET to quad y-coords.
  // Top of top stave
  float maxY = QUAD_Y_OFFSET + m_staves.front()->GetY() + 4 * STAVE_LINE_GAP; 
  // Bottom of bottom stave.
  float minY = QUAD_Y_OFFSET + m_staves.back()->GetY(); 

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

