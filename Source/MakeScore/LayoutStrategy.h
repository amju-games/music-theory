#pragma once

class Bar;

class LayoutStrategy
{
public:
  virtual ~LayoutStrategy() = default;

  // Calc the width of the note zone of the given bar. 
  virtual float CalcNoteZoneWidth(const Bar& bar) = 0;

  // Set the positions of the glyphs within the given bar.
  virtual void PositionGlyphs(Bar& bar) = 0;
};

