// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include "Glyph.h"

// Time sigs are always left-aligned, no offset
struct TimeSigGlyph : public Glyph
{
  TimeSigGlyph(const std::string& s) : Glyph()
  {
    realGlyphName = s;
    displayGlyphName = s;
    y = 0;
    HandleStar();
  }
};


