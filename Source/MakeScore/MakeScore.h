// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Bar.h"
#include "Glyph.h"
#include "Stave.h"
#include "Tie.h"
#include "TimeSigGlyph.h"
#include "TimeValue.h"

class MakeScore
{
public:
  MakeScore(const std::string& in) : m_input(in) { }

  std::string ToString();
  void MakeInternal();
  void SetScale(float scale) { m_scale = scale; }
  void SetY(float y) { m_y = y; }

  // Add beam groupings
  // Replace beamed quaver/semiquaver glyphs with crotchet glyphs
  void Preprocess();

  void SetOutputOneLine(bool oneLine)
  {
    m_outputOnOneLine = oneLine;
  }

  void SetStaveType(StaveType st) 
  {
    m_staveType = st;
  }

private:

  void AddTimeSig(const std::string& s)
  {
    m_bars.back()->AddTimeSig(s);
  }

  void AddGlyph(const std::string& s)
  {
    m_bars.back()->AddGlyph(s);

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

  void AddBeam(const std::string& s)
  {
    m_bars.back()->AddBeam(s);
  }

  void AddTie()
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

  void AddTokens();
  void CalcBarSizesAndPositions();
  void CalcStartTimes();

private:
  // If true, all glyphs on one line, separated by ';'
  // Else each is on a separate line.
  bool m_outputOnOneLine = false;

  // Scale of all glyphs
  float m_scale = 1.0f;

  float m_y = 0;

  std::string m_input;

  // Ordered sequence of bars in the score.
  std::vector<std::unique_ptr<Bar>> m_bars;

  // Ties connect glyphs which can be in different bars, so ties are not
  //  per-bar.
  std::vector<std::unique_ptr<Tie>> m_ties;

  StaveType m_staveType = StaveType::STAVE_TYPE_RHYTHM;
};

