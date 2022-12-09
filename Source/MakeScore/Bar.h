// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <memory>
#include <vector>
#include "Beam.h"
#include "Glyph.h"
#include "Stave.h"
#include "TimeSig.h"
#include "TimeSigGlyph.h"
#include "TimeValue.h"
#include "Utils.h"

struct Bar
{
  // Sequence of glyphs, left to right, in the bar, following any key sig
  //  and time sig.
  std::vector<std::unique_ptr<Glyph>> m_glyphs;

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
  int m_currentStave = 0;
  
  // Time sig: we can use this to check for errors, and do beams/groups
  //  automatically. Also, we can use it to set times for each glyph,
  //  for animation and midi events.
  TimeSig m_timeSig = TimeSig::FOUR_FOUR;

  void SetTimeSig(TimeSig ts);

  TimeSig GetTimeSig() const;

  // Return the total time for the bar.
  // The time val of all the glyph members should add up to this.
  TimeValue GetDuration() const;

  float CalcNormalisedTimes(float totalDuration, float start);

  void SetScale(float scale);

  // Get GuiMusicScore glyph string from short code
  static std::string GetStr(std::string s);

  void AddGlyph(const std::string& s, int pitch);

  void AddTimeSig(const std::string& s);

  void AddBeam(const std::string& s);

  std::string ToString(bool oneLine);

  // Get number of distinct glyphs horizontally
  // (e.g. a chord is only one 'glyph' as all the notes take up only 
  //  one horizontal space/share the same stem)
  int GetGlyphCount() const;

  void SetWidth(int totalNumGlyphs, float pageWidth);

  float GetWidth() const;

  // x is the left edge of the bar.
  // From this and the width, we can set the final x-coord of each glyph.
  // y is an offset added to the y-coord of each glyph (all the same for
  //  rhythm scores). 
  void SetPos(float x, float y);

  // Calc y-position for glyph, given stave type, pitch and (TODO) clef
  float CalcGlyphY(int pitch) const;
};