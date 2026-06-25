// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include "Glyph.h"
#include "Performance.h"
#include "Suppress.h"

Glyph::Glyph(const std::string& inputToken, int order_) :
  m_order(order_),
  realGlyphName(inputToken)
{
  // Calc output (display) text later, but if we want to hide this glyph
  //  for a question (e.g. 'what is this note?'), do it now.
  HandleStar();
}

std::string Glyph::ToString() const
{
  return displayGlyphName + ", " + Str(x) + ", " + Str(y) +
    AddScaleStringIfRequired();
}

void Glyph::NormaliseTimes(float scale)
{
  GetTimes().Normalise(scale);
}

void Glyph::HandleStar()
{
  if (Contains(realGlyphName, '*'))
  {
    Remove(realGlyphName, '*');
    displayGlyphName = '*';
  }
}

