// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <map>
#include <memory>
#include <vector>
#include "Accidental.h"
#include "Beam.h"
#include "Chord.h"
#include "Clef.h"
#include "Consts.h"
#include "Glyph.h"
#include "KeySig.h"
#include "NoteGlyph.h"
#include "Pitch.h"
#include "TimeSig.h"
#include "TimeSigGlyph.h"
#include "TimeValue.h"
#include "Utils.h"

enum class BarLine
{
  BAR_LINE_NOT_SET,
  BAR_LINE_SINGLE,
  BAR_LINE_DOUBLE,
  BAR_LINE_END,
  BAR_LINE_REPEAT_START,
  BAR_LINE_REPEAT_END
};

// Return true if given string is a bar line input token
bool IsBarLine(const std::string&);

// Convert given string to bar line type
BarLine GetBarLine(const std::string&);

// * Bar *
// Holds glyphs for one bar on one stave
class Bar
{
public:
  // Copy state which carries over from one bar to the next, e.g. 
  //  time sig, key sig, etc.
  void CopyState(const Bar& b);

  void SetIsFirstBarOfLine(bool first) { m_isFirstBarOfLine = first; }

  // Get x-coord for bar line at the end of this bar.
  float GetBarLineX() const;

  BarLine GetBarLine() const { return m_barLine; }
  void SetBarLine(BarLine b) { m_barLine = b; }

  void SetTimeSig(TimeSig ts);

  TimeSig GetTimeSig() const;

  // Get number of beats in this bar.
  // Calc on first call for this bar, then store the result.
  int GetNumBeats() const;

  void SetKeySig(KeySig ks);

  KeySig GetKeySig() const;

  // Return the total time for the bar, in crotchet units.
  // Used to get the total duration for the whole piece.
  TimeValue GetDuration() const;

  // Calc normalised start times and durations for meta data
  void CalcNormalisedTimes(float totalDurationOfPiece);

  void SetScale(float scale);

  float AddNote(const std::string& durationToken, Pitch pitch, int switches,
    float startTimeValue);

  float AddChord(const Chord& ch, int switches,
    float startTimeValue);

  float AddRest(const std::string& durationToken, int switches, 
    float startTimeValue, bool isWholeBar);

  void AddTimeSig(const std::string& token);

  void AddBeam(const std::string& s);

  void SetClef(Clef clef);

  // Return string comprising multiple lines, each line a separate glyph.
  std::string ToString();

  // Get the rough width for the bar by adding up the sequential
  //  glyphs, and adding extra for clefs, key sigs, time sigs.
  // Used by MakeScore to set the width of each bar.
  float GetRelativeWidth() const;

  // Work out width for this bar, given the total width of all bars, 
  //  and the desired page width.
  void CalcWidth(float totalWidth, float pageWidth);

  // Used by MakeScore to work out how much of the page width each bar gets
  float GetWidth() const;

  // x is the left edge of the bar.
  // From this and the width, we can set the final x-coord of each glyph.
  // y is an offset added to the y-coord of each glyph (all the same for
  //  rhythm scores). 
  void SetPos(float x, float y);

  // Only required to get final glyph, for Ties. TODO replace with
  //   GetFinalGlyph
  const GlyphVec& GetGlyphs() const { return m_glyphs; }

private:
  // Ret true if we should show clef at front of bar
  bool YesShowClefAtFrontOfBar() const;

  float GetKeySigWidth() const;

  std::unique_ptr<NoteGlyph> CreateNoteGlyph(
    const std::string& durationToken,
    Pitch pitch,
    int switches,
    int yOrder,
    float crotchetTime);

  std::unique_ptr<ChordGlyph> CreateChordGlyph(
    const Chord& ch, 
    int switches,
    int xOrder,
    float crotchetTime);

private:
  // Sequence of glyphs, left to right, in the bar, following any key sig
  //  and time sig.
  GlyphVec m_glyphs;

  // Optional key sig glyph for the bar, at left edge.
  // TODO Handle 'naturalising' key sig before a new key sig, i.e. there
  //  could be up to 2 key sig glyphs. Also, key sigs can be a lot wider
  //  than other glyphs.
  std::unique_ptr<Glyph> m_keySigGlyph;

  // Optional time sig glyph, at left of bar after key sig, if there is one.
  std::unique_ptr<Glyph> m_timeSigGlyph;

  // Beams connecting ordered glyphs
  std::vector<std::unique_ptr<Beam>> m_beams;

  float m_x = 0;
  float m_y = 0;
  float m_width = 0;
  float m_scale = 1.0f;

  // Time sig: we can use this to check for errors, and do beams/groups
  //  automatically. Also, we can use it to set times for each glyph,
  //  for animation and midi events.
  TimeSig m_timeSig = TimeSig::TIME_SIG_NONE;

  // Clef for each stave. Output clef at start of a new line, and a mini-clef
  //  when the clef changes on a stave.
  Clef m_currentClef = Clef::CLEF_TREBLE;

  bool m_isFirstBarOfLine = false;
  // Output mini clef at end of bar if the clef changed
  bool m_yesOutputMiniClef = false; 

  KeySig m_keySig = KEYSIG_0_SHARP;

  // Last Accidental used for each stave position (an int, can go negative
  //  with ledger lines). We use this so we don't repeat accidentals
  //  unnecessarily, know when to cancel, etc.
  std::map<int, Accidental> m_accidentals; 

  BarLine m_barLine = BarLine::BAR_LINE_NOT_SET;

  // Number of beats in this bar - calculated on first call to GetNumBeats().
  mutable int m_numBeats = -1; // -1 means not calculated yet
};

