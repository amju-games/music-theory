// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>
#include "Bar.h"
#include "IGlyph.h"

enum class StaveType
{
  // No stave line
  STAVE_TYPE_NONE,

  // Single line, for rhythm notation with no pitch
  STAVE_TYPE_PERCUSSION,

  // Single 5-line stave
  STAVE_TYPE_STANDARD,
};

// Look up StaveType from given string
StaveType GetStaveType(const std::string& s);

// Return true if this string defines a stave type
bool IsStaveType(const std::string& s);

class Attachment;
struct Tie;

class Stave : public IGlyph
{
public:
  static void SetPageWidth(float);
  static float GetPageWidth();

  void SetType(StaveType); 

  // Get height of this stave, including all notes on the stave. So any note
  //  on ledger lines pushes the height up.
  float GetHeight() const;

  // For above to work, when we add a note, increase the height of this stave.

  std::string ToString() const override;
  std::string CommentString() const override;

  // Output time marker for each beat; may be unnecessary.
  std::string OutputBeats() const;

  void CalcBarSizesAndPositions();

  void CalcStartTimes();

  void AddBar();
  void AddFirstBar();
  int GetNumBars() const;
  const Bar& GetBar(int i) const;
  Bar& GetCurrentBar();

  void AddTie();

  float AddNote(const std::string& timeToken, const Pitch& pitch, float crotchetTime);
  float AddChord(const Chord& chord, float crotchetTime);
  float AddRest(const std::string& s, float crotchetTime);

  void AddKeySig(const std::string& s);
  void AddClef(const std::string& s);
  void AddTimeSig(const std::string& s);

  // Attach t to the last glyph added, if there is one; set left or
  //  right parent.
  void Attach(Attachment* t, int leftOrRight = 0);

  void SetPerformance(int performanceSwitch) {}
  void ClearPerformance(int performanceSwitch) {}

private:
  int GetTotalNumBeats() const;

private:
  // Ordered sequence of bars in the score.
  std::vector<std::unique_ptr<Bar>> m_bars;

  StaveType m_type = StaveType::STAVE_TYPE_STANDARD;

  // Height -- ratchets up as notes on ledger lines are added
  float m_h;

  // Ties connect glyphs which can be in different bars, so ties are not
  //  per-bar.
  // TODO Absorb this into m_otherGlyphs
  std::vector<std::unique_ptr<Tie>> m_ties;

  // Bit field for staccato, accent, pause, etc., per stave
  int m_switches = 0;

  int m_transpose = 0; // transposition, in semitones

  static float s_pageWidth;
};

