// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <set>
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
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  GuiMusicScore* Clone() override { return new GuiMusicScore(*this); }

  GuiMusicScore();
 
  virtual void Draw() override;
  virtual bool Load(File*) override;
  virtual void Animate(float animValue) override;
  virtual void OnResetAnimation() override;

  virtual Rect CalcRect() const override;

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

  // Add a curve from a string, if the first token in line is special
  //  curve token. If so, return true. Return false if line does not
  //  specify a curve.
  bool AddCurveFromString(
    const std::string& line,
    const Vec2f& pos = Vec2f(0, 0),
    const Vec2f& scale = Vec2f(1.f, 1.f));

  // Create text child if first token specifies this type.
  bool AddTextFromString(
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

    Glyph(int ch, const Vec2f& pos, const Colour& col) :
      m_char(ch), m_pos(pos), m_colour(col) {}

    Glyph(int ch, const Vec2f& pos, const Vec2f& scale, const Colour& col) :
      m_char(ch), m_pos(pos), m_scale(scale), m_colour(col) {}

    Glyph(const Vec2f corners[4], const Colour& col);

    void SetTimeMinMax(const Vec2f& tmm) { m_timeMinMax = tmm; }

    int m_char = 0;
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

  bool HasAnimation() const { return m_hasAnimation; }
  
protected:
  // Draw child GUI elements (curves, text, etc)
  void DrawChildren();

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

  // Parse strings to set min/max time, which is then set on all subsequent glyphs
  bool ParseTime(const Strings& strs);

  // parse note on/off 'glyphs'
  bool ParseNoteOn(const Strings& strs);
  bool ParseNoteOff(const Strings& strs);
  bool ParseNoteEvent(const Strings& strs, bool onNotOff);

  // For text child nodes, set font, which will be used for all text
  //  until changed.
  bool ParseFont(const Strings& strs);
  
  // Check for new note events, send them, and advance m_nextNoteEvent appropriately.
  void UpdateNoteEvents(float animValue);

  struct NoteEvent;
  void SendNoteEvent(const NoteEvent&);

protected:
  RCPtr<TriList> m_triList; 
  RCPtr<TextureSequence> m_atlas; // music font - actual image is a resource.
  Colour m_fgCol; // default colour for all glyphs
  Colour m_highlightColour;

  using Glyphs = std::vector<Glyph>;
  Glyphs m_glyphs;

  // Set of glyphs which were highlighted last frame - so we only refresh colours when
  //  necessary. The ints are indices into m_glyphs.
  using HighlightedSet = std::set<int>;
  HighlightedSet m_highlightedSet;

  // Look up table from compound glyph name to multiple glyphs to which we should expand.
  static std::map<std::string, std::string> s_compoundGlyphs;

  FullScreenPostProcess m_fullscreenRenderer;
  
  // Current min/max time, which we set for all subsequently added glyphs.
  // This can be set with a special glyph name.
  Vec2f m_timeMinMax = Vec2f(-1.f, -1.f);
  
  // Set to true if above time range is set between 0..1, as this means we will
  //  animate some glyphs.
  bool m_hasAnimation = false;

  // Note on/off events, which we load along with glyphs. This makes it easier to
  //  add midi note events to a score.
  struct NoteEvent
  {
    NoteEvent() = default;
    NoteEvent(int note, float time, bool onNotOff) :
      m_note(note), m_time(time), m_onNotOff(onNotOff) {}

    bool operator<(const NoteEvent& ne) const
    {
      return m_time < ne.m_time;
    }

    // Midi note value
    int m_note = 0;
    // Times are 0..1 animation values, not time in seconds
    float m_time = 0;
    // MIDI note on or note off event
    bool m_onNotOff = true;
  };

  // Vector of note events, sorted by time.
  using NoteEvents = std::vector<NoteEvent>;
  NoteEvents m_noteEvents;
  // Index into m_noteEvents. This is the next note event to be played, when the
  //  animation value reaches the time of the event
  int m_nextNoteEvent = 0;

  // Bounding rect of all glyphs, without scale and position applied
  Rect m_rect;

  // Child GUI elements: This lets us add curves, use for ties, slurs, etc.,
  //  text, etc.
  std::vector<RCPtr<GuiElement>> m_children;
};
}


