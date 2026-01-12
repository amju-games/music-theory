// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <optional>
#include <set>
#include <Colour.h>
#include <RenderToTexture.h> // Render larger, for anti-aliasing
#include <GuiElement.h>
#include <StringUtils.h>
#include <TextureSequence.h>
#include <TriList.h>
#include "NoteEvent.h"
#include "Palette.h"

namespace Amju
{
// * Beat *
// Normalised time for a beat in the piece
struct Beat
{
  float m_time = 0; // (normalised)
  int m_bar = 1; // bar number (1-based)
  int m_beat = 1; // beat number in bar (1-based)

  Beat(float time, int bar, int beat) : 
    m_time(time), m_bar(bar), m_beat(beat) {}

  bool operator<(const Beat& b) { return m_time < b.m_time; }
};

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

  // Load the GUI info - size, position, name, etc.
  // Loading the actual score at this point is not very useful, so
  //  we have a separate function to do that.
  virtual bool Load(File*) override;

  virtual void Animate(float animValue) override;
  virtual void OnResetAnimation() override;

  virtual Rect CalcRect() const override;

  // Load music from file. For now, this is a text file of glyphs,
  //  and then in future we can support MakeScore input and MusicXML.
  // We use File::ReportError to report errors up to the author - we 
  //  don't want to assert and crash the game.
  virtual bool LoadMusicScore(const std::string& filename);

  // Load glyph data from an open file. 
  virtual bool LoadMusicScore(File* file);

  // Set default fg colour, i.e. the colour for glyphs, quads, etc. 
  // Needs to be called before loading so the colour is applied to
  //  subsequently parsed glyphs.
  void SetFgCol(const Colour& col);

  // Set palette to use to colourise the score.
  // The 'colour name' in the palette corresponds here to MIDI note
  //  values, (stringified).
  // Once palette is set, glyphs subsequently parsed will use it. So
  //  this call needs to come before the score is loaded.
  void SetPalette(RCPtr<Palette> palette);

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
    // Any glyph parsed will be highlighted when the animation value is within 
    //  this range.
    Vec2f m_timeMinMax = Vec2f(-1.f, -1.f);

    // Glyph colour: usually we would expect this to be black, but we can highlight 
    //  or pulse symbols etc.
    // We can also colour code notes.
    Colour m_colour = Colour(0.f, 0.f, 0.f, 1.f);

    // For quads, store the 4 corners 
    // TODO Quads should be a different type. Also we want LineSegs.
    Vec2f m_corner[4];
  };

  // Add a Glyph: when all required Glyphs have been added, call BuildTriList().
  void AddGlyph(const Glyph& g);

  // Used during parsing/loading: get the colour for the glyph we are
  //  just about to add. (We use current note data member to decide). 
  Colour GetColourForGlyph() const;

  // For testing, unlikely to be useful otherwise?
  int GetNumGlyphs() const;
  Glyph& GetGlyph(int);

  // Set min/max time, which will then be applied to all Glyphs subsequently added.
  // This is to simplify parsing each Glyph - we don't need to specify min/max time
  //  for every Glyph this way.
  void SetMinMaxTime(float tMin, float tMax);

  // Returns true if the score contains time markers for animation.
  bool HasAnimation() const { return m_hasAnimation; }
 
  // Add meta data telling us there is a beat at normalized time t.
  void AddBeat(const Beat& beat);
 
  // Set the tempo in Beats Per Minute.
  void SetBpm(float bpm);
  // Get tempo, (for when it's set from loaded data)
  float GetBpm() const;

  // Returns song length, if known - i.e. we need the metadata in the input.
  std::optional<float> GetSongLengthSeconds() const;
 
  const NoteEvents& GetNoteEvents() const;

  using NoteEventCallback = std::function<void(const NoteEvent&)>;
  void SetNoteEventCallback(NoteEventCallback cb);

protected:
  // Draw child GUI elements (curves, text, etc)
  void DrawChildren(); // not const because GuiElement::Draw not const, why tho

  void DrawBoundingRect() const;
  void DrawIndividualGlyphRects() const;

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

  // Set colour of all subsequent glyphs until changed.
  bool ParseColour(const Strings& strs);

  // Tempo and beat meta data 
  bool ParseBpm(const Strings& strs);
  bool ParseBeat(const Strings& strs);
  
  // Check for new note events, send them, and advance m_nextNoteEvent appropriately.
  void UpdateNoteEvents(float animValue);

  virtual void SendNoteEvent(const NoteEvent&);

protected:
  RCPtr<TriList> m_triList; 
  RCPtr<TextureSequence> m_atlas; // music font - actual image is a resource.
  Colour m_fgCol; // usual colour for all glyphs
  Colour m_highlightColour; // colour for highlighted part of score
  Colour m_bgCol; // background colour of score rect
  RCPtr<Palette> m_palette; // optional palette to colourise notes

  using Glyphs = std::vector<Glyph>;
  Glyphs m_glyphs;

  // Set of glyphs which were highlighted last frame - so we only refresh colours when
  //  necessary. The ints are indices into m_glyphs.
  using HighlightedSet = std::set<int>;
  HighlightedSet m_highlightedSet;

  // Look up table from compound glyph name to multiple glyphs to which we should expand.
  static std::map<std::string, std::string> s_compoundGlyphs;

  // Render to texture, for anti-aliasing.
  RCPtr<RenderToTexture> m_rtt;
  
  // Current min/max time, which we set for all subsequently added glyphs
  //  during the Loading phase.
  // This time range is set multiple times while loading a sequence of glyphs,
  //  rather than setting the time range for every glyph individually.
  Vec2f m_timeMinMax = Vec2f(-1.f, -1.f);
  
  // Set to true during Load phase if the above time range is ever set between 0..1, 
  //  as this means we will animate some glyphs.
  bool m_hasAnimation = false;

  // Most recent note (MIDI pitch) parsed.
  // This can be used to set colour of the glyphs for the note. 
  int m_lastNoteParsed = -1; 

  // MIDI note events - loaded as special glyphs.
  NoteEvents m_noteEvents;
  // Index into m_noteEvents. This is the next note event to be played, when the
  //  animation value reaches the time of the event
  int m_nextNoteEvent = 0;

  // Bounding rect of all glyphs, without scale and position applied
  Rect m_rect;

  // Child GUI elements: This lets us add curves, use for ties, slurs, etc.,
  //  text, etc.
  // TODO Wrap in decorators to set colour, and possibly time?
  std::vector<RCPtr<GuiElement>> m_children;

  // BPM tempo for the score, read in at load time.
  float m_bpm = 0.f; // 0 means not known

  // Meta data: the normalized time of every beat in the piece.
  std::vector<Beat> m_beats; 

  NoteEventCallback m_noteEventCallback;
};
}

