#pragma once

#include "LayoutStrategy.h"

// This layout strategy minimises horizonal space, so does not
//  space out glyphs according to their duration. This is a bad strategy
//  for multi-stave output, and for readability generally.
class LayoutMinSpacing : public LayoutStrategy
{
public:
  // Calc the width of the note zone of the given bar. 
  float CalcNoteZoneWidth(const Bar& bar) override;

  // Set the positions of the glyphs within the given bar.
  void PositionGlyphs(Bar& bar) override;
};

