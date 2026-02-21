// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
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
#include "Pitch.h"
#include "Stave.h"
#include "Tie.h"
#include "TimeSigGlyph.h"
#include "TimeValue.h"

class MakeScore
{
public:
  MakeScore() = default;

  void SetInputString(const std::string& in) { m_input = in; }
  // Create with input string
  MakeScore(const std::string& in) : m_input(in) { }

  // Load input from a file, ignoring comment lines
  bool Load(const std::string& filename);

  int NumBars() const;

  void SetTranspose(int semitones) { m_transpose = semitones; }
  void SetPageWidth(float pw) { m_pageWidth = pw; }

  std::string ToString();
  void MakeInternal();

  // For testing
  const Strings& GetOutputLines() const { return m_outputLines; }

  void SetScale(float scale) { m_scale = scale; }
  void SetY(float y) { m_y = y; }

  // Preprocess input string before tokenisation: is this necessary?
  // Add beam groupings?
  // Replace beamed quaver/semiquaver glyphs with crotchet glyphs?
  void Preprocess();

  // Tokenise input string
  std::vector<std::string> Tokenise();

  // Preprocess tokens before parsing:
  // Move tokens outside of chord markers
  void PreprocessTokens(std::vector<std::string>& tokens);

  // Parse tokens - music notation tokens only
  void Parse(std::vector<std::string>& tokens);
  
  // Parse global style/page setting tokens
  void ParseGlobalSettings(std::vector<std::string>& tokens);

  // Strip unnecessary TIME data
  void Postprocess(); 

  void SetOutputOneLine(bool oneLine)
  {
    m_outputOnOneLine = oneLine;
  }

private:
  void ToStringInternal(); // Populate m_outputStrings

  // Output bar lines
  std::string BarLinesToString();

  bool IsHairpin(const std::string& s);

  bool IsSlur(const std::string& s);

  bool IsPerformance(const std::string& s);

  // Add a performance direction attachment
  void AddDirection(const std::string& s);

  float GetYForDirection() const;

  // Add a text string attachment
  void AddText(const std::string& s);

  // Use most recently set note (time) value and pitch to add a new NoteGlyph
  //  to the current Bar.
  // crotchetTime is time since start of piece, in crotchet units, at which
  //  this note should be added.
  // Returns new crotchet time value for subsequent glyphs.
  float AddNote(float crotchetTime);

  // Add chord (built up of pitches and time values within chord
  //  markers).
  // crotchetTime is time since start of piece, in crotchet units, at which
  //  this note should be added.
  // Returns new crotchet time value for subsequent glyphs.
  float AddChord(const Chord& chord, float crotchetTime);

  // Add a rest, with given duration value, at given time.
  // crotchetTime is time since start of piece, in crotchet units, at which
  //  this note should be added.
  // Returns new crotchet time value for subsequent glyphs.
  float AddRest(const std::string& duration, float crotchetTime);

  void AddClef(const std::string& s);

  void AddKeySig(const std::string& s);

  void AddStave();

  // Create a new Tie, setting the left glyph to the most recently added glyph.
  void AddTie();

  void CalcBarSizesAndPositions();
  void CalcStartTimes();

  std::string OutputBeats() const;

  Stave& GetCurrentStave();

private:
  // If true, all glyphs on one line, separated by ';'
  // Else each is on a separate line.
  bool m_outputOnOneLine = false;

  // Scale of all glyphs
  float m_scale = DEFAULT_SCALE;

  float m_y = 0;

  std::string m_input;

  std::vector<std::unique_ptr<IGlyph>> m_otherGlyphs;

  // Most recently set note (time) value
  std::string m_lastTimeValToken = INPUT_TOKEN_CROTCHET;

  // Most recently set pitch
  Pitch m_lastPitch;

  int m_transpose = 0;

  float m_pageWidth = DEFAULT_PAGE_WIDTH;

  // Last slur attachment created, for setting the second (right hand)
  //  parent, if required (slurs need two parents, one for each
  //  end of the curve).
  Attachment* m_lastSlur = nullptr;

  // Last hairpin created, also so we can set right hand parent
  Attachment* m_lastHairpin = nullptr;

  // All staves
  std::vector<std::unique_ptr<Stave>> m_staves;

  Strings m_outputLines;

  float m_bpm = 0.f;
};

