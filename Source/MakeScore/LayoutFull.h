#pragma once

#include "LayoutStrategy.h"

// This layout strategy positions glyphs according to their start time
//  within the bar, and sets the overall bar width according to the
//  total duration of the bar.
// This is a good strategy for vertically aligning multiple staves.
class LayoutFull : public LayoutStrategy
{
public:
  // Calc the width of the note zone of the given bar. 
  float CalcNoteZoneWidth(const Bar& bar) override;

  // Set the positions of the glyphs within the given bar.
  void PositionGlyphs(Bar& bar) override;
};

