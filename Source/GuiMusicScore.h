// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <Colour.h>
#include <GuiElement.h>
#include <TextureSequence.h>
#include <TriList.h>

namespace Amju
{
// * GuiMusicScore *
// Display music notation.
class GuiMusicScore : public GuiElement
{
public:
  GuiMusicScore();
 
  virtual void Draw() override;
  virtual bool Load(File*) override;

  void SetFgCol(const Colour& col);

protected:
  void BuildTriList();

protected:
  RCPtr<TriList> m_triList; 
  //PFont m_font;
  TextureSequence m_atlas;
  Colour m_fgCol;

  // Internal representation: vec of glyphs, each having position and scale
  //  (scale for stave and bar lines primarily)
  struct Glyph
  {
    Glyph() = default;

    Glyph(char ch, const Vec2f& pos, const Vec2f& scale = Vec2f(1.f, 1.f)) :
      m_char(ch), m_pos(pos), m_scale(scale) {}

    char m_char = 'q';
    Vec2f m_pos;
    Vec2f m_scale;
  };
  using Glyphs = std::vector<Glyph>;
  Glyphs m_glyphs;
};
}


