// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include "Glyph.h"

// * RestGlyph *
class RestGlyph : public Glyph
{
  RestGlyph() = default;

  // Construct glyph from inpupt token and position in bar.
  RestGlyph(const std::string& inputToken, int order) :
    Glyph(inputToken, order) {}

  // Generate TIME special glyphs (for animation - we do want to highlight rests,
  //  but not generate MIDI events.)
  std::string TimeBefore() const override;
  std::string TimeAfter() const override;

  std::string ToString() const override;

  // Use input token and state to generate output text for this glyph.
  // TODO Doesn't need param and can set displayGlyphName directly
  std::string GetGlyphOutputStr(std::string s) const override;
};
