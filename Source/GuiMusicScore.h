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
  static void AddToFactory();

  GuiMusicScore();
 
  virtual void Draw() override;
  virtual bool Load(File*) override;

  void SetFgCol(const Colour& col);

  // Used internally but may be used with AddGlyph()
  void BuildTriList();

  // One glyph: has character, position and scale.
  // A glyph can also be an arbitrary shaped filled quad, for beams and lines.
  struct Glyph
  {
    Glyph() = default;

    Glyph(char ch, const Vec2f& pos, const Vec2f& scale = Vec2f(1.f, 1.f)) :
      m_char(ch), m_pos(pos), m_scale(scale) {}

    Glyph(const Vec2f corners[4]);

    char m_char = 'q';
    Vec2f m_pos;
    Vec2f m_scale;

    // For quads, store the 4 corners 
    Vec2f m_corner[4];
  };

  // Add a Glyph: when all required Glyphs have been added, call BuildTriList().
  void AddGlyph(const Glyph& g);

protected:

  // Add one quad (2 triangles) to the given vec of tris, made from the
  //  4 corners supplied. I.e. can be any quad shape.
  void MakeQuad(const Vec2f corners[4], AmjuGL::Tris& tris);

  // Make a stave from 5 quads: better quality than stretching a glyph
  void MakeStave(const Glyph& g, AmjuGL::Tris& tris);

protected:
  RCPtr<TriList> m_triList; 
  TextureSequence m_atlas;
  Colour m_fgCol;

  using Glyphs = std::vector<Glyph>;
  Glyphs m_glyphs;
};
}


