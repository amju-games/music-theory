// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017
//
// * MakeScore *
//
// Sub project to convert easily-authorable music content
//  into list of glyphs.
//
// To build:
//    (Mac) clang++ *.cpp -std=c++11
//    (Win) cl *.cpp
//
// E.g. use: 
//    (Mac) echo '4/4 c c mr' | ./a.out
// Same for Win, although we need double quotes and internally we need to strip off the quotes:
//    (Win) echo "4/4 c c mr" | MakeScore.exe


// Music score coords:
//  +--------------------------+   0.4
//  +--------------------------+
//  +--------------------------+
//  +--------------------------+
//  +--------------------------+   0.0
// 0.0                     PAGE_WIDTH

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "MakeScore.h"
#include "Pitch.h"
#include "TimeSig.h"
#include "TimeValue.h"
#include "Utils.h"

float GetHeight(BeamLevel bl)
{
  // Relies on the int values 0, 1...
  return static_cast<float>(bl);
}

std::string LineEnd(bool oneLine)
{
  return (oneLine ? ";" : "\n");
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
  for (int i = 0; i < n; i++)
  {
    const std::string& s = strs[i];

    if (s == "|")
    {
      Bar* bar = new Bar;
      bar->SetStaveType(m_staveType);
      bar->SetScale(m_scale);
      // Copy time sig over to next bar
      bar->SetTimeSig(m_bars.back()->GetTimeSig());
      // TODO Copy key sig over
      //  bar->SetKeySig(m_bars.back()->GetKeySig());

      m_bars.push_back(std::unique_ptr<Bar>(bar));
    }
    else if (s == "t")
    {
      // Tie prev and next notes - we can get position of prev.
      AddTie();
    }
    else if (IsBeam(s))
    {
      AddBeam(s);
    }
    else if (IsTimeSig(s))
    {
      AddTimeSig(s);
    }
    else if (IsDeferredPitch(s))
    {
      m_lastPitch = GetPitch(s);
    }
    else if (IsImmediatePitch(s))
    {
      m_lastPitch = GetPitch(s);
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
  }
}

void MakeScore::AddGlyph()
{
  m_bars.back()->AddGlyph(m_lastTimeValToken, m_lastPitch);

  // If last tie has no right connection, connect it now to the
  //  glyph we just added.
  if (!m_ties.empty())
  {
    Tie* tie = m_ties.back().get();
    if (!tie->IsRhsSet())
    {
      assert(!m_bars.empty());
      tie->SetRightGlyph(m_bars.back()->m_glyphs.back().get());
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
  tie->SetLeftGlyph(m_bars.back()->m_glyphs.back().get());

  tie->SetScale(m_scale);

  m_ties.push_back(std::unique_ptr<Tie>(tie));
}

void MakeScore::MakeInternal()
{
  // Add first default bar
  Bar* bar = new Bar;
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
    int gc = bar->GetGlyphCount(); 
    totalNumGlyphs += gc;
  }

  // Bar calculates its width as fraction of PAGE_WIDTH
  for (auto& bar : m_bars)
  {
    bar->SetWidth(totalNumGlyphs, PAGE_WIDTH);
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
    tie->SetPos(); 
  }
}

std::string MakeScore::ToString()
{
  std::string res;

  // First, output a stave. For rhythm only, it's a single line.
  // TODO Multiple lines 

  res += GetStaveString(m_staveType, 0, m_y, PAGE_WIDTH, m_scale);
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

  return res;
}

void CommandLineParams(int argc, char** argv, MakeScore& ms)
{
  for (int i = 1; i < argc; i++)
  {
    std::string param = argv[i];

    if (param == "--oneline")
    {
      // All on one line
      ms.SetOutputOneLine(true);
    }

    if (param == "--stave-single")
    {
      ms.SetStaveType(StaveType::STAVE_TYPE_SINGLE);
    }
  }
}

#ifndef CATCH
// Don't build this main function for unit test exe

int main(int argc, char** argv)
{
  std::string input;
  std::getline(std::cin, input);

  // Chop off whitespace and quotes - needed for Win, not Mac
  Trim(input);
  StripQuotes(input);

std::cout << "// " << input << "\n"; 

  MakeScore ms(input);

  CommandLineParams(argc, argv, ms);

  ms.SetScale(0.6f);

  // For single line rhythm, centre vertically
  // TODO Auto centre 1 or more lines
  ms.SetY(0.5f);

  // TODO Transform input:
  // Add beam groupings
  // Replace beamed quaver/semiquaver glyphs with crotchet glyphs
  ms.Preprocess();

  ms.MakeInternal();

  // Output final string.
  // Don't append a newline char, so we can add more to this line, in
  //  enclosing script.
  std::cout << ms.ToString();

  return 0;
}

#endif // CATCH

