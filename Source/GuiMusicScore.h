// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <Colour.h>
#include <FullScreenPostProcess.h> // Render larger, to improve quality
#include <GuiElement.h>
#include <StringUtils.h>
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
  virtual void Animate(float animValue) override;

  void SetFgCol(const Colour& col);

  // Add glyph defined by a string.
  bool AddGlyphFromString(
    const std::string& line,
    const Vec2f& pos = Vec2f(0, 0),
    const Vec2f& scale = Vec2f(1.f, 1.f));

  // Add multiple glyphs, using ; as the delimiter.
  // Each glyph position is offset by pos, and scaled by the given scale.
  bool AddMultipleGlyphsFromString(
    const std::string& line, 
    const Vec2f& pos = Vec2f(0, 0), 
    const Vec2f& scale = Vec2f(1.f, 1.f));

  // Used internally but may be used with AddGlyph()
  void BuildTriList();

  // Call to refresh vertex buffer when any Glyph colours are changed
  void RefreshColours();

  // One glyph: has character, position and scale.
  // A glyph can also be an arbitrary shaped filled quad, for beams and lines.
  struct Glyph
  {
    Glyph() = default;

    Glyph(char ch, const Vec2f& pos, const Colour& col) :
      m_char(ch), m_pos(pos), m_colour(col) {}

    Glyph(char ch, const Vec2f& pos, const Vec2f& scale, const Colour& col) :
      m_char(ch), m_pos(pos), m_scale(scale), m_colour(col) {}

    Glyph(const Vec2f corners[4], const Colour& col);

    void SetTimeMinMax(const Vec2f& tmm) { m_timeMinMax = tmm; }

    char m_char = 'q';
    Vec2f m_pos;
    Vec2f m_scale = Vec2f(1.f, 1.f);

    // This is used to highlight glyphs as the owning GuiMusicScore is animated.
    // This glyph is highlighted while the animation value is within this range.
    Vec2f m_timeMinMax = Vec2f(-1.f, -1.f);

    // Glyph colour: usually we would expect this to be black, but we can highlight 
    //  or pulse symbols etc.
    Colour m_colour = Colour(0.f, 0.f, 0.f, 1.f);

    // For quads, store the 4 corners 
    Vec2f m_corner[4];
  };

  // Add a Glyph: when all required Glyphs have been added, call BuildTriList().
  void AddGlyph(const Glyph& g);

  // For testing, unlikely to be useful otherwise?
  int GetNumGlyphs() const;
  Glyph& GetGlyph(int);

protected:

  // Add one quad (2 triangles) to the given vec of tris, made from the
  //  4 corners supplied. I.e. can be any quad shape.
  void MakeQuad(const Vec2f corners[4], AmjuGL::Tris& tris, const Colour& col);

  // Convenience function: set the colour attribs on all the verts in a quad
  static void SetQuadColour(AmjuGL::Tri t[2], const Colour& col);

  // Expand out a "compound" glyph, adding the multiple result glyphs to m_glyphs, offset 
  //  and scaled with the params of the compound glyph. 
  // E.g. a note head plus its tail, which are 2 different glyphs, but we want to treat 
  //  as a single glyph when authoring glyph strings.
  bool ExpandCompoundGlyph(const Strings& tokens, const Vec2f& pos, const Vec2f& scale);

  // Returns true if given string is a "compound" glyph, in which case we expand it.
  bool IsCompoundGlyphName(const std::string& glyphName) const;

  // Create a Glyph from the given string.
  bool ParseGlyph(const std::string& line, Glyph* result, const Vec2f& pos, const Vec2f& scale);

  // Populate s_compoundGlyphs -- called once
  static void LoadCompoundGlyphs();

  static void OneTimeInit();

protected:
  RCPtr<TriList> m_triList; 
  TextureSequence m_atlas; // music font - actual image is a resource.
  Colour m_fgCol; // default colour for all glyphs
  Colour m_hightlightColour;

  using Glyphs = std::vector<Glyph>;
  Glyphs m_glyphs;

  // Look up table from compound glyph name to multiple glyphs to which we should expand.
  static std::map<std::string, std::string> s_compoundGlyphs;

  FullScreenPostProcess m_fullscreenRenderer;
};
}


