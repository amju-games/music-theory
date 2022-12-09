// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
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
  const int LEFT = 0;
  const int RIGHT = 1;
  void SetParent(IGlyph* parent, int i = 0) { m_parent[i] = parent; }

  void SetGlyphText(const std::string& text) { m_glyphText = text; }

  std::string ToString() const override;

protected:
  IGlyph* m_parent[2] = { nullptr, nullptr };
  std::string m_glyphText;
};

