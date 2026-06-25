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

class LayoutStrategy;

// * Bar *
// Holds glyphs for one bar on one stave. Conceptually split into 
//  pre-note zone and note zone. The notes etc in the note zone are
//  positioned by a LayoutStrategy object. The stuff in the pre-note
//  zone is fixed.
class Bar
{
public:
  // Set the strategy used to position glyphs within bars.
  static void SetLayoutStrategy(LayoutStrategy*);
  static LayoutStrategy* GetLayoutStrategy();

  // Set the coord of each glyph within this bar.
  // Delegates the work to a Layout Strategy object, set depending on 
  //  desired positioning strategy.
  void PositionGlyphs();

  // Copy state which carries over from one bar to the next, e.g. 
  //  time sig, key sig, etc. 
  // Not just a regular copy ctor because we don't want to copy _all_ of the
  //  state.
  void CopyState(const Bar& b);

  void SetIsFirstBarOfLine(bool first) { m_isFirstBarOfLine = first; }

  // Get x-coord for bar line at the end of this bar.
  float GetBarLineX() const;

  // Bar line type.
  BarLine GetBarLine() const { return m_barLine; }
  void SetBarLine(BarLine b) { m_barLine = b; }

  // Time signature -- this is the logical time sig, not necessarily
  //  output as a glyph.
  void SetTimeSig(TimeSig ts);
  TimeSig GetTimeSig() const;

  // Get number of beats in this bar.
  // Calc on first call for this bar, then store the result.
  int GetNumBeats() const;

  // Key sig -- like time sig, this is the logical value, not what is
  //  going to be displayed necessarily.
  void SetKeySig(KeySig ks);
  KeySig GetKeySig() const;

  // Return the total time duration for the bar, in crotchet units.
  // Used to get the total duration for the whole piece.
  TimeValue GetDuration() const;

  // Set start time of first beat of bar, in crotchet units.
  void SetStartTime(TimeValue tv) { m_startTime = tv; }

  // Get start time of first beat of bar, in crotchet units.
  TimeValue GetStartTime() const { return m_startTime; }

  // Find beam groups in this bar. 
  // One beam group is a run of notes less than one crotchet in duration each,
  //  connected by a primary beam and possibly secondary and broken beams.
  // Not all runs can be connected as beam groups, depending on beats in 
  //  this bar.
  // The output of this function is to add beams to m_beams, and to adjust
  //  stems of member note glyphs.
  void MakeBeamGroups();

  // Calc normalised start times and durations. This is so we can output 
  //  meta data to play the piece.
  void CalcNormalisedTimes(float totalDurationOfPiece);

  void SetScale(float scale);

  float AddNote(const std::string& durationToken, Pitch pitch, int switches,
    float startTimeValue);

  float AddChord(const Chord& ch, int switches,
    float startTimeValue);

  float AddRest(const std::string& durationToken, int switches, 
    float startTimeValue, bool isWholeBar);

  void AddTimeSig(const std::string& token);

  void SetClef(Clef clef);

  // Assuming one clef set at the start of the bar, or a cue clef at the
  //  end of the previous bar. We don't support changing clef mid way
  //  through the bar.
  Clef GetClef() const { return m_currentClef; }

  // Return string comprising multiple lines, each line a separate glyph.
  std::string ToString();

  // Get the width for the bar by adding up the pre-note zone and glyphs.
  // This delegates to the Layout Strategy because the note zone layout
  //  strategy determines the width.
  float GetRelativeWidth() const;

  // Work out width for this bar, given the total relative width of all bars, 
  //  and the desired page width.
  void CalcWidth(float totalWidth, float pageWidth, float widthScale);

  // Get width calculated above - this is in final renderable units.
  float GetWidth() const;

  // Set position of bar.
  // x is the left edge of the bar.
  // y is the position of the bottom line of a 5-line stave (TODO check that)
  // The units are final, renderable units.
  void SetPos(float x, float y);
  // Get the position set above.
  float GetX() const { return m_x; }
  float GetY() const { return m_y; }

  // Get the glyphs in the note zone, for inspection only.
  const GlyphVec& GetGlyphs() const { return m_glyphs; }

  // Get the glyphs in the note zone, to set positions.
  GlyphVec& GetGlyphs() { return m_glyphs; }

  // Return string of given bar number and the top left coords of this bar.
  // So client code can draw all or some bar numbers.
  std::string BarNumberString(int barNum) const;

  // Create the string for the pre-note part of this bar. Also
  //  return the width of the pre-note zone.
  // TODO This is a first pass hack -- we should calc the string and width
  //  once as soon as we are able, and set member variables with the result.
  std::pair<std::string, float> PreNoteZoneToString() const;

private:
  // Position glyphs across bar after 'fixed' elements like clef and time
  //  sig have taken up some of the width of the bar.
  void PositionGlyphs(float leftX, float bottomStaveLineY,
    float noteZoneWidth);

  // For bars with single glyphs that should be centred
  void CentreSingleGlyph(float leftX, float noteZoneWidth);
  
  // Ret true if we should show clef at front of bar
  bool YesShowPreNoteZoneClef() const;

  // Return true if we should show key sig in pre-note zone.
  bool YesShowPreNoteZoneKeySig() const;

  // Get width of key sig (in pre-note zone)
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

  // Optional key sig glyph for the bar, in the pre-note zone.
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

  // Clef for each stave. Output clef at start of a new line, and a cue-clef
  //  when the clef changes on a stave.
  Clef m_currentClef = Clef::CLEF_TREBLE;

  bool m_isFirstBarOfLine = false;

  // Output clef in pre-note zone?
  bool m_yesOutputPreNoteZoneClef = false; 

  bool m_yesOutputPreNoteZoneKeySig = false; 

  KeySig m_keySig = KEYSIG_0_SHARP;

  // Last Accidental used for each stave position (an int, can go negative
  //  with ledger lines). We use this so we don't repeat accidentals
  //  unnecessarily, know when to cancel, etc.
  std::map<int, Accidental> m_accidentals; 

  BarLine m_barLine = BarLine::BAR_LINE_NOT_SET;

  // Number of beats in this bar - calculated on first call to GetNumBeats().
  mutable int m_numBeats = -1; // -1 means not calculated yet

  // Start time of first beat of bar, in crotchet units
  TimeValue m_startTime = 0;
};

