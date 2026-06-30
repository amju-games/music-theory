#pragma once

class Bar;
class Glyph;

class LayoutStrategy
{
public:
  virtual ~LayoutStrategy() = default;

  // Calc the width of the note zone of the given bar. 
  virtual float CalcNoteZoneWidth(const Bar& bar) = 0;

  // Set the positions of the glyphs within the given bar.
  virtual void PositionGlyphs(Bar& bar) = 0;

protected:
  // Utility function: return the total width of the given glyph.
  // E.g. an unardorned crotchet would give a width of NOTE_HEAD_WIDTH.
  // This is used to avoid overlaps when notes/chords have accidentals.
  static float GetTotalWidth(Glyph&);
};

