// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
//
// Sub project to convert easily-authorable music content
//  into list of glyphs.
//

#pragma once

#include "IGlyph.h"

// This kind of glyph is attached to one or two parent glyphs, which
//  give it its position. 
class Attachment : public IGlyph
{
public:
  static const int LEFT = 0;
  static const int RIGHT = 1;
  void SetParent(IGlyph* parent, int i = 0) { m_parent[i] = parent; }

  void SetGlyphText(const std::string& text) { m_glyphText = text; }

  std::string ToString() const override;

  bool IsRhsSet() const { return m_parent[RIGHT] != nullptr; }

protected:
  IGlyph* m_parent[2] = { nullptr, nullptr };

  // Set left and (optionally) right x-coords once positions of both glyphs have been
  //  set.
  float m_leftX = 0;
  float m_rightX = 0;

  std::string m_glyphText;
};

