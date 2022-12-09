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

#include "Attachment.h"
#include "Bar.h"
#include "Consts.h"
#include "Glyph.h"
#include "Stave.h"
#include "Tie.h"
#include "TimeSigGlyph.h"
#include "TimeValue.h"

class MakeScore
{
public:
  MakeScore(const std::string& in) : m_input(in) { }

  void SetTranspose(int semitones) { m_transpose = semitones; }
  void SetPageWidth(float pw) { m_pageWidth = pw; }

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

  bool IsHairpin(const std::string& s);

  bool IsSlur(const std::string& s);

  bool IsSwitch(const std::string& s);

  // Attach t to the last glyph added, if there is one; set left or
  //  right parent.
  void Attach(Attachment* t, int leftOrRight = 0);

  // Add a performance direction attachment
  void AddDirection(const std::string& s);

  // Add a text string attachment
  void AddText(const std::string& s);

  void AddTimeSig(const std::string& s)
  {
    m_bars.back()->AddTimeSig(s);
  }

  // Use most recently set note (time) value and pitch to add a new Glyph
  //  to the current Bar.
  void AddGlyph();

  void AddClef(const std::string& s);

  void AddKeySig(const std::string& s);

  void AddBeam(const std::string& s)
  {
    m_bars.back()->AddBeam(s);
  }

  // Create a new Tie, setting the left glyph to the most recently added glyph.
  void AddTie();

  void AddTokens();
  void CalcBarSizesAndPositions();
  void CalcStartTimes();

private:
  // If true, all glyphs on one line, separated by ';'
  // Else each is on a separate line.
  bool m_outputOnOneLine = false;

  // Scale of all glyphs
  float m_scale = DEFAULT_SCALE;

  float m_y = 0;

  std::string m_input;

  // Ordered sequence of bars in the score.
  std::vector<std::unique_ptr<Bar>> m_bars;

  // Ties connect glyphs which can be in different bars, so ties are not
  //  per-bar.
  // TODO Absorb this into m_otherGlyphs
  std::vector<std::unique_ptr<Tie>> m_ties;

  std::vector<std::unique_ptr<IGlyph>> m_otherGlyphs;

  StaveType m_staveType = StaveType::STAVE_TYPE_RHYTHM;

  // Most recently set note (time) value
  std::string m_lastTimeValToken = INPUT_TOKEN_CROTCHET;

  // Most recently set pitch, which is a MIDI note value.
  int m_lastPitch = DEFAULT_PITCH;

  int m_transpose = 0;

  float m_pageWidth = DEFAULT_PAGE_WIDTH;

  // Last slur attachment created, for setting the second (right hand)
  //  parent, if required (slurs need two parents, one for each
  //  end of the curve).
  Attachment* m_lastSlur = nullptr;

  // Last hairpin created, also so we can set right hand parent
  Attachment* m_lastHairpin = nullptr;

  // Current stave
  int m_stave = 0;

  // Bit field for staccato, accent, pause, etc., per stave
  int m_switches[MAX_NUM_STAVES] = { 0, 0, 0, 0 };
};

