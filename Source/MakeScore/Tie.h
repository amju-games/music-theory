// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include "IGlyph.h"

struct Glyph;

struct Tie : public IGlyph
{
  virtual std::string ToString() const;

  void SetPos();

  bool IsRhsSet() const { return m_rightGlyph != nullptr; }

  void SetLeftGlyph(Glyph* left) { m_leftGlyph = left; }
  void SetRightGlyph(Glyph* right) { m_rightGlyph = right; }

  // When we create Tie, we only know the glyph on the left end.
  // Later on we know the next note, then we have the right end too.
  Glyph* m_leftGlyph = nullptr;
  Glyph* m_rightGlyph = nullptr;

  // Set left and right x-coords once positions of both glyphs have been
  //  set.
  float m_leftX = 0;
  float m_rightX = 0;
};
