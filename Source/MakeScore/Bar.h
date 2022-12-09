// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <map>
#include <memory>
#include <vector>
#include "Accidental.h"
#include "Beam.h"
#include "Clef.h"
#include "Consts.h"
#include "Glyph.h"
#include "KeySig.h"
#include "Stave.h"
#include "TimeSig.h"
#include "TimeSigGlyph.h"
#include "TimeValue.h"
#include "Utils.h"

using GlyphVec = std::vector<std::unique_ptr<Glyph>>;

class Bar
{
public:
  // Copy state which carries over from one bar to the next, e.g. 
  //  time sig, key sig, etc.
  void CopyState(const Bar& b);

  void SetIsFirstBarOfLine(bool first) { m_isFirstBarOfLine = first; }

  void SetTimeSig(TimeSig ts);

  TimeSig GetTimeSig() const;

  void SetKeySig(KeySig ks);

  KeySig GetKeySig() const;

  // Return the total time for the bar.
  // The time val of all the glyph members should add up to this.
  TimeValue GetDuration() const;

  float CalcNormalisedTimes(float totalDuration, float start);

  void SetScale(float scale);

  void AddGlyph(const std::string& s, int pitch, int switches);

  void AddTimeSig(const std::string& s);

  void AddBeam(const std::string& s);

  void SetClef(Clef clef);

  std::string ToString(bool oneLine);

  // Get the rough width for the bar by adding up the sequential
  //  glyphs, and adding extra for clefs, key sigs, time sigs.
  // Used by MakeScore to set the width of each bar.
  float GetRelativeWidth() const;

  // Called by MakeScore. Work out width for this bar, given the total
  //  number of glyphs on the line, and the page width.
  void CalcWidth(int totalNumGlyphs, float pageWidth);

  // Used by MakeScore to work out how much of the page width each bar gets
  float GetWidth() const;

  // x is the left edge of the bar.
  // From this and the width, we can set the final x-coord of each glyph.
  // y is an offset added to the y-coord of each glyph (all the same for
  //  rhythm scores). 
  void SetPos(float x, float y);

  // Calc y-position for glyph, given stave type and pitch. (We then offset
  //  this later on, depending on the clef.)
  void CalcGlyphY(Glyph* gl, int pitch) const;

  void SetStaveType(StaveType st) { m_staveType = st; }

  // Only required to get final glyph, for Ties. TODO replace with
  //   GetFinalGlyph
  const GlyphVec& GetGlyphs() const { return m_glyphs; }

private:
  // Ret true if we should show clef at front of bar
  bool YesShowClefAtFrontOfBar() const;

  float GetKeySigWidth() const;

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

  // Stave type for the bar (I would expect this to be the same for
  //  all bars, at least in the same line).
  StaveType m_staveType = StaveType::STAVE_TYPE_RHYTHM;

  float m_x = 0;
  float m_y = 0;
  float m_width = 0;
  float m_scale = 1.0f;

  // Current stave to which we add glyphs, clef, etc.
  int m_currentStave = 0;

  // Time sig: we can use this to check for errors, and do beams/groups
  //  automatically. Also, we can use it to set times for each glyph,
  //  for animation and midi events.
  TimeSig m_timeSig = TimeSig::TIME_SIG_NONE;

  // Clef for each stave. Output clef at start of a new line, and a mini-clef
  //  when the clef changes on a stave.
  Clef m_currentClef[MAX_NUM_STAVES] = { Clef::CLEF_TREBLE };

  bool m_isFirstBarOfLine = false;
  // Output mini clef at end of bar if the clef changed
  bool m_yesOutputMiniClef = false; 

  KeySig m_keySig = KEYSIG_0_SHARP;

  // Last Accidental used for each stave position (an int, can go negative
  //  with ledger lines). We can use this so we don't repeat accidentals
  //  unnecessarily, know when to cancel, etc.
  std::map<int, Accidental> m_accidentals; 
};
