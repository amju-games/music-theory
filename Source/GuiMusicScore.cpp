// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <mutex> // call_once
#include <unordered_map> // map glyph names to ascii codes

#include <BmFont.h>
#include <DrawRect.h>
#include <GuiFactory.h>
#include <GuiText.h>
#include <ReportError.h>
#include <StringsFile.h>
#include <StringUtils.h>
#include "Consts.h"
#include "GuiSpline.h"
#include "GuiMusicScore.h"
#include "PlayMidi.h"

// Draw bounding rect for entire score
//#define DEBUG_DRAW_BOUNDING_RECT

// Draw bounding rect of every glyph
//#define DEBUG_DRAW_GLYPH_RECTS

#define USE_BM_FONT

namespace Amju
{
#ifdef USE_BM_FONT
const char* FONT_FILE_NAME = "font2d/Guido2compressed/guido2_0.png";
#else
const char* FONT_FILE_NAME = "font2d/Guido2/guido2-60pt.png";
#endif

#ifdef DEBUG_DRAW_GLYPH_RECTS
static std::vector<Rect> glyphRects;
#endif

const int QUAD_CHAR = 1;

// This non-printable character is used for quads, which are a special case.
// Used for stave lines, bar lines, beams etc.
const char* QUAD_NAME = "quad";

// This is for setting the min/max time for subsequent glyphs
const char* TIME_NAME = "TIME";

// For adding curve (LineDrawing) children
const char* CURVE_NAME = "curve";

// For adding text children
const char* TEXT_NAME = "text";
const char* FONT_NAME = "text_font";
const char* FONT_SIZE = "text_size";

// Note on/note off 'meta glyphs'
const char* NOTE_ON_NAME  = "NOTE_ON";
const char* NOTE_OFF_NAME = "NOTE_OFF";

const char* END_TOKEN = "end";

std::map<std::string, std::string> GuiMusicScore::s_compoundGlyphs;

const char* GuiMusicScore::NAME = "music-score";

GuiMusicScore::Glyph::Glyph(const Vec2f corner[4], const Colour& col) : 
  m_char(QUAD_CHAR),
  m_colour(col)
{
  for (int i = 0; i < 4; i++)
  {
    m_corner[i] = corner[i];
  }
}

// Look up character in font from human-readable name
static bool GlyphNameToCh(const std::string& s, int* ch)
{
  // Single character names just map to that character, e.g. a dot "." is character '.' in music score font.
  if (s.size() == 1)
  {
    *ch = s[0];
    return true;
  }

  // TODO These can all go in the compound glyphs text file
  static const std::unordered_map<std::string, int> NAMES = 
  {
    { "quad", QUAD_CHAR }, // special (non-printable) character to represent a quad
    { "triplet", '!' },
    { "sharp", '#' },
    { "segno", '%' },
    { "treble-clef", '&' },
    { "fff", '\'' },
    { "rest-5", '/' }, // come back to naming these
    { "bass-clef", '?' },
    { "rest-2", '@' },
    { "alto-clef", 'B' }, // or is that tenor clef in relation to stave?
    { "cut-common", 'C' },
    { "rest-minim", 'D' },
    { "note-minim", 'E' },
    { "mf", 'F' },
    { "double-flat", 'H' },
    { "tail-up-1", 'J' },
    { "tail-up-2", 'K' },
    { "mordent", 'M' }, // half mordent?
    { "dot", 'N' },
    { "mp", 'P' },
    { "sf", 'S' },
    { "turn", 'T' },
    { "pause", 'U' },
    { "pause-above", 'U' },
    { "breve", 'W' },
    { "note-solid", 'X' },
    { "note-full", 'X' },
    { "fz", 'Z' },
    { "end-left", '[' },
    { "end-right", '\\' },
    { "repeat-left", ']' },
    { "bow-up", '^' },
    { "rest-1", 'a' }, // quaver rest - 1 blob
    { "flat", 'b' },
    { "common", 'c' },
    { "ds", 'd' },
    { "d.s.", 'd' },
    // quad defined in compound glyphs //{ "bar-line", 'e' },
    { "f", 'f' },
    { "gliss", 'g' },
    { "tail-down-1", 'j' },
    { "tail-down-2", 'k' },
    { "rest-4", 'l' },
    { "trill", 'm' }, // mordent?
    { "natural", 'n' },
    { "p", 'p' },
    { "crotchet", 'q' },
    { "note", 'q' },
    { "note-up", 'q' }, // tail goes up
    { "ffff", 's' },
    { "pause-below", 'u' },
    { "bow-down", 'v' },
    { "semibreve", 'w' },
    { "whole-note", 'w' },
    { "note-1", 'w' }, // use 1 for whole note?
    { "repeat-right", '}' },
    { "letter-a", 128 },
    { "letter-b", 129 },
    { "letter-c", 130 },
    { "letter-d", 131 },
    { "letter-e", 132 },
    { "letter-f", 133 },
    { "letter-g", 134 },
    { "small-1", 161 },
    { "small-3", 163 },
    { "small-4", 162 },
    { "small-5", 166 },
    { "small-6", 167 },
    { "small-7", 182 },
    { "small-9", 170 },
    { "small-0", 186 },
    { "rest-crotchet", 165 },
    { "rest-3", 174 },
    { "ped", 176 },
    { "double-sharp", 180 },
    { "trill", 181 },
    { "tr", 188 },
    { "bracket-sharp", 189 },
    { "bracket-flat", 190 },
    { "bracket-natural", 192 },
    { "ff", 193 },
    { "pp", 194 },
    { "pppp", 195 },
    { "ppp", 199 },
    { "tail-down-4", 201 },
    { "tail-up-4", 202 },
    { "bracket-double-sharp", 213 },
    { "8vb", 216 },
    { "tail-down-3", 221 },
    { "tail-up-3", 222 },
    { "turn-inverted", 229 },
    { "coda", 230 },

    // etc 
  };
  auto it = NAMES.find(s);
  if (it != NAMES.end())
  {
    *ch = it->second;
    return true;
  }
  return false;
}

void GuiMusicScore::LoadCompoundGlyphs()
{
  s_compoundGlyphs.clear();
  Strings strs;
  LoadStrings("Gui/compound_glyphs.txt", &strs);
  // Each line is split by = sign
  for (auto& s : strs)
  {
    Strings p = Split(s, '='); // pair, split by = sign
    Assert(p.size() == 2);
    s_compoundGlyphs.insert(std::make_pair(Trim(p[0]), Trim(p[1])));
  }
}

void GuiMusicScore::OneTimeInit()
{
  // Compound glyphs: load from file. 
  LoadCompoundGlyphs();
}

GuiMusicScore::GuiMusicScore()
{
  m_rect = EmptyRect();

  // Create texture atlas. TODO CONFIG
  // Image is a resource, only loaded once.
#ifdef USE_BM_FONT
  static RCPtr<BmFontTextureSequence> bm;
  if (!bm)
  {
    bm = new BmFontTextureSequence;
    Texture* tex = (Texture*)TheResourceManager::Instance()->GetRes(FONT_FILE_NAME);
    bm->Set(tex, 1, 1, 1, 1);
    bm->LoadBmFont("font2d/Guido2compressed/guido2.fnt");
  }
  m_atlas = bm.GetPtr();
#else
  m_atlas = new TextureSequence;
  m_atlas->Load(FONT_FILE_NAME, 16, 14, 1, 1);
#endif

  m_fgCol = Colour(0, 0, 0, 1); // default to black

  // Default highlight colour
  m_highlightColour = GetColour(COLOUR_MUSIC_HIGHLIGHT);

#ifdef _DEBUG
  // We can do this just once to optimise; for development, we want to initialise every time.
  OneTimeInit();
#else
  // One time init for all Music Scores.
  static std::once_flag flag;
  std::call_once(flag, OneTimeInit);
#endif

#ifdef USE_RTT
  // TODO Share for all Music Scores
  m_fullscreenRenderer.InitFullScreenQuad();
  RenderToTexture* rtt = dynamic_cast<RenderToTexture*>(AmjuGL::Create(RenderToTexture::DRAWABLE_TYPE_ID));
  Assert(rtt);

  rtt->SetRenderFlags(RenderToTexture::AMJU_RENDER_COLOUR_WITH_ALPHA);
  //rtt->SetSize(128, 128); // // TODO TEMP TEST - shold be multiple of screen resolution?
  rtt->SetSize(1024, 1024);
  rtt->SetClearColour(Colour(0, 0, 0, 0)); // 0 alpha, so we see through most of it, black is normally the colour of music score.
  rtt->Init();

  m_fullscreenRenderer.SetRenderTarget(rtt);
#endif // USE_RTT
}

void GuiMusicScore::SendNoteEvent(const NoteEvent& ne)
{
  if (ne.m_onNotOff)
  {
    // TODO better MIDI API
    PlayMidi(ne.m_note, MIDI_NOTE_MAX_VOLUME);
  }
  else
  {
    PlayMidi(ne.m_note, 0);
  }
}

void GuiMusicScore::UpdateNoteEvents(float animValue)
{
  if (!IsVisible())
  {
    return;
  }

  if (m_noteEvents.empty())
  {
    return;
  }

  if (m_nextNoteEvent >= static_cast<int>(m_noteEvents.size()))
  {
    return; // we have exhausted all note events - reset in ResetAnimation()
  }

  const NoteEvent* ne = &m_noteEvents[m_nextNoteEvent];
  while (   m_nextNoteEvent < static_cast<int>(m_noteEvents.size()) 
         && ne->m_time <= animValue)
  {
    // Fire off this event!
    SendNoteEvent(*ne);
    m_nextNoteEvent++;
    ne++;
  }
}

void GuiMusicScore::OnResetAnimation()
{
  m_nextNoteEvent = 0;
}

void GuiMusicScore::Animate(float animValue)
{
  // Keep track of glyphs we highlight, and compare with previous frame
  HighlightedSet thisHighlightedSet;

  int numGlyphs = m_glyphs.size();
  for (int i = 0; i < numGlyphs; i++)
  {
    Glyph& g = m_glyphs[i];
    Colour col = m_fgCol;
    if (g.m_timeMinMax.x <= animValue && g.m_timeMinMax.y > animValue)
    {
      // Add this glyph to highlighted set

      // Set glyph colour to highlight colour
      col = m_highlightColour;
      thisHighlightedSet.insert(i);
    }
    g.m_colour = col;
  }

  // Send MIDI note events
  UpdateNoteEvents(animValue);

  // If highlight set has changed, refresh colours
  if (m_highlightedSet != thisHighlightedSet)
  {
    RefreshColours();
    m_highlightedSet = thisHighlightedSet;
  }
}

void GuiMusicScore::Draw()
{
  if (!m_triList)
  {
    BuildTriList();
  }

  Vec2f pos = GetCombinedPos();
  Vec2f size = GetSize();

#ifdef USE_RTT
  RenderToTexture* rtt = m_fullscreenRenderer.GetRenderTarget();
  rtt->Begin();
#endif

  m_atlas->Bind();
  AmjuGL::PushMatrix();
  AmjuGL::Translate(pos.x, pos.y, 0);
  AmjuGL::Scale(size.x, size.y, 1);
  AmjuGL::Draw(m_triList);

  DrawChildren();

#ifdef DEBUG_DRAW_BOUNDING_RECT
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::SetColour(Colour(1, 1, 0, 1));
  AmjuGL::UseShader(nullptr);
  DrawRect(m_rect);
#endif // DEBUG_DRAW_BOUNDING_RECT

#ifdef DEBUG_DRAW_GLYPH_RECTS
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::SetColour(Colour(0, 1, 0, 1));
  AmjuGL::UseShader(nullptr);
  for (const Rect& r : glyphRects)
  {
    DrawRect(r);
  }
#endif

  AmjuGL::PopMatrix();

#ifdef USE_RTT
  rtt->End();
  m_fullscreenRenderer.DrawFullScreenQuad();
#endif
}

void GuiMusicScore::DrawChildren()
{
  for (auto child : m_children)
  {
    child->Draw();
  }
}

void GuiMusicScore::AddGlyph(const Glyph& cg)
{
  if (cg.m_char != 0)
  {
    auto g(cg);
    g.SetTimeMinMax(m_timeMinMax);
    m_glyphs.push_back(g);
  }
}

bool GuiMusicScore::IsCompoundGlyphName(const std::string& glyphName) const
{
  return s_compoundGlyphs.find(glyphName) != s_compoundGlyphs.end();
}

bool GuiMusicScore::ExpandCompoundGlyph(const Strings& strs, const Vec2f& pos_, const Vec2f& scale_)
{
  int n = strs.size();
  if (n != 3 && n != 5)
  {
    ReportError("Unexpected compound glyph format (missing position?)");
    return false;
  }
  // Look up the compound glyph string for this name, 
  //  e.g. "minim-up" => "minim... ; quad..."
  const std::string& compoundStr = s_compoundGlyphs[strs[0]];
  // Get the position and scale, which we will apply to all expanded glyphs
  Vec2f pos = Vec2f(ToFloat(strs[1]), ToFloat(strs[2])) + pos_;
  Vec2f scale(scale_);
  if (n == 5)
  {
    scale.x *= ToFloat(strs[3]);
    scale.y *= ToFloat(strs[4]);
  }
  // Add the glyphs in the expanded string, applying the new pos and scale
  AddMultipleGlyphsFromString(compoundStr, pos, scale);

  return true;
}

bool GuiMusicScore::AddMultipleGlyphsFromString(const std::string& line, const Vec2f& pos, const Vec2f& scale)
{
  // Multiple glyphs split by ;
  Strings strs = Split(line, ';');
  for (const std::string& s : strs)
  {
    // Allow empty string, so generating score content is simpler
    if (Trim(s).empty())
    {
      continue;
    }

    Strings tokens = Split(s, ',');
    Assert(tokens.size() > 0);

    // s is a single glyph string, or could be a "compound name".
    // Check first string: it could be a "compound name", which expands out to multiple glyphs.
    if (tokens[0] == END_TOKEN)
    {
      return false;
    }
    else if (IsCompoundGlyphName(tokens[0]))
    {
      if (!ExpandCompoundGlyph(tokens, pos, scale))
      {
        ReportError("Failed to expand compound glyph: " + s);
        Assert(0);
        return false;
      }
    }
    else if (!AddGlyphFromString(s, pos, scale))
    {
      ReportError("Failed to set score glyph: " + s);
      Assert(0);
      return false;
    }
  }
  return true;
}

bool GuiMusicScore::ParseTime(const Strings& strs)
{
  if (strs.size() != 3)
  {
    ReportError("Bad number of params for time.");
    return false;
  }
  float t1 = ToFloat(strs[1]);
  float t2 = ToFloat(strs[2]);
  if (t2 < t1)
  {
    ReportError("Bad params for time, second time earlier than first.");
    return false;
  }
  
  // If the time for a glyph is between 0 and 1, then this Score is animated.
  if (   (t1 > 0 && t1 < 1)
      || (t2 > 0 && t2 < 1))
  {
    m_hasAnimation = true;
  }
  
  m_timeMinMax = Vec2f(t1, t2);
  return true;
}

bool GuiMusicScore::ParseNoteEvent(const Strings& strs, bool onNotOff)
{
  if (strs.size() != 3)
  {
    ReportError("Bad number of params for note on event.");
    return false;
  }
  int note = ToInt(strs[1]);
  float time = ToFloat(strs[2]);
  m_noteEvents.push_back(NoteEvent(note, time, onNotOff));

  return true;
}

bool GuiMusicScore::ParseNoteOn(const Strings& strs)
{
  return ParseNoteEvent(strs, true);
}

bool GuiMusicScore::ParseNoteOff(const Strings& strs)
{
  return ParseNoteEvent(strs, false);
}

bool GuiMusicScore::ParseGlyph(const std::string& line, GuiMusicScore::Glyph* result, const Vec2f& pos_, const Vec2f& scale_)
{
  // Split line. Format OK? Has optional scale?
  Strings strs = Split(line, ',');
  int n = strs.size();

  // Quads are a special case
  if (strs[0] == TIME_NAME)
  {
    return ParseTime(strs);
  }
  else if (strs[0] == NOTE_ON_NAME)
  {
    return ParseNoteOn(strs);
  }
  else if (strs[0] == NOTE_OFF_NAME)
  {
    return ParseNoteOff(strs);
  }
  else if (strs[0] == QUAD_NAME)
  {
    if (n != 9)
    {
      ReportError("Bad number of params for quad: " + line);
      return false;
    }
    
    // 4 corners
    Vec2f corners[4] =
    {
      (Vec2f(ToFloat(strs[1]), ToFloat(strs[2])) + pos_) * scale_,
      (Vec2f(ToFloat(strs[3]), ToFloat(strs[4])) + pos_) * scale_,
      (Vec2f(ToFloat(strs[5]), ToFloat(strs[6])) + pos_) * scale_,
      (Vec2f(ToFloat(strs[7]), ToFloat(strs[8])) + pos_) * scale_
    };
    *result = Glyph(corners, m_fgCol);
    return true;
  }
  else if (n == 3 || n == 5)
  {
    int ch;

    if (!GlyphNameToCh(strs[0], &ch))
    {
      ReportError("Unexpected music glyph name: " + strs[0]);
      return false;
    }
    Vec2f pos(ToFloat(strs[1]), ToFloat(strs[2]));
    if (n == 3)
    {
      Vec2f p = (pos + pos_) * scale_;
      *result = Glyph(ch, p, scale_, m_fgCol);
    }
    else
    {
      Vec2f scale = Vec2f(ToFloat(strs[3]), ToFloat(strs[4])) * scale_;
      Vec2f p = (pos + pos_) * scale;
      *result = Glyph(ch, p, scale, m_fgCol);
    }
    return true;
  }
  ReportError("Failed to parse line " + line);
  return false;
}

bool GuiMusicScore::AddTextFromString(
  const std::string& line, const Vec2f& pos_, const Vec2f& scale_)
{
  if (line.size() > 1 && line[0] == '"')
  {
    // Search from end of string to find terminating quote
    int p = line.rfind('"');
    Assert(p != std::string::npos);
    std::string str = line.substr(1, p - 1);

    // Parse remainder of line to get pos and scale
    // Add 2 to p to skip end quote and hopefully following comma
    Strings strs = Split(Trim(line.substr(p + 2)), ',');
    Assert(!strs.empty());
    // Remove empty string in case there were extra spaces between quote and comma
    if (strs[0].empty())
    {
      strs.erase(strs.begin()); // "pop front"
    }
    // Now we should have position and optional scale
    Assert(strs.size() == 2 || strs.size() == 4);
    Vec2f pos(ToFloat(strs[0]), ToFloat(strs[1]));
    Vec2f scale(1, 1);
    if (strs.size() == 4)
    {
      scale = Vec2f(ToFloat(strs[2]), ToFloat(strs[3]));
    }
    scale *= scale_;
    pos = (pos + pos_) * scale;

    GuiText* t = new GuiText;
    Font* font = (Font*)TheResourceManager::Instance()->GetRes("font2d/times-font.font");
    t->SetFont(font);
    t->SetFgCol(m_fgCol);
    // Scale text to work with music glyphs
    const float X_SCALE_MULT = 2.5f;
    const float Y_SCALE_MULT = 3.f;
    t->SetFontSize(scale.y * Y_SCALE_MULT);
    t->SetScaleX(scale.x  * X_SCALE_MULT / scale.y);
    t->SetLocalPos(pos);
    t->SetSize(Vec2f(2, 2));
    t->SetJust(GuiText::AMJU_JUST_LEFT);
    t->SetText(str);
    t->SizeToText();

    m_children.push_back(t);

    return true;
  }

  return false;
}

bool GuiMusicScore::AddCurveFromString(
  const std::string& line, const Vec2f& /* pos */, const Vec2f& /* scale */)
{
  Strings strs = Split(line, ',');
  int n = strs.size();

  if (strs[0] == CURVE_NAME)
  {
    // Following strings should be vec2 control points, i.e. in pairs.
    if (n % 2 != 1)
    {
      ReportError("Bad curve, expected even number of params");
      Assert(0);
    }

    GuiSpline* curve = new GuiSpline;
    curve->SetSize(Vec2f(1, 1));

    // Set thickness at ends and in middle
    curve->SetWidths(0.006f, 0.012f);
    
    // Set rounded texture
    PTexture tex = (Texture*)TheResourceManager::Instance()->GetRes("Image/circle.png");
    curve->SetTexture(tex);
    tex->SetWrapMode(AmjuGL::AMJU_TEXTURE_CLAMP);

    curve->SetColour(m_fgCol);

    for (int i = 1; i < n; i += 2)
    {
      Vec2f v(ToFloat(strs[i]), ToFloat(strs[i + 1]));
      curve->AddControlPoint(v); 
    }
    curve->CreateFromControlPoints(); // create full shape from control points

    m_children.push_back(curve);
    return true;
  }
 
  return false;
}

bool GuiMusicScore::AddGlyphFromString(const std::string& line, const Vec2f& pos, const Vec2f& scale)
{
  if (AddCurveFromString(line, pos, scale))
  {
    // This 'glyph' is a curve, e.g. a tie
    return true;
  }

  if (AddTextFromString(line, pos, scale))
  {
    return true;
  }

  Glyph g;
  if (!ParseGlyph(line, &g, pos, scale))
  {
    return false;
  }

  AddGlyph(g);
  return true;
}

bool GuiMusicScore::Load(File* f)
{
  // Get name, pos, size
  if (!GuiElement::Load(f))
  {
    return false;
  }

  // Colour
  std::string colour;
  if (!f->GetDataLine(&colour))
  {
    f->ReportError("Expected music score colour");
    return false;
  }
  m_fgCol = FromHexString(colour);

  // Load the score to display, not the texture atlas
  // Format is: each line specifies one glyph or the end of the score data.
  // Each line has: glyph type, x pos, y pos, with optional x scale, y scale.
  // Glyph type will be from an allowed set of names, so we report an error
  // if the name is not recognised. The names are just a way to make the
  // characters human-readable.
  std::string line;
  while (f->GetDataLine(&line))
  {
    if (!AddMultipleGlyphsFromString(line))
    {
      break;
    }
  }
  return true;
}

void GuiMusicScore::SetFgCol(const Colour& col)
{
  m_fgCol = col;

  // Set this colour on each glyph
  int numGlyphs = m_glyphs.size();
  for (int i = 0; i < numGlyphs; i++)
  {
    Glyph& g = m_glyphs[i];
    g.m_colour = m_fgCol;
  }
  m_highlightedSet.clear();

  RefreshColours();
}

void GuiMusicScore::MakeQuad(const Vec2f corners[4], AmjuGL::Tris& tris, const Colour& col)
{
  AmjuGL::Tri t[2];

  const float Z = 0.0f;
  float u = .999f;
  float v = .001f; // TODO texture atlas should be opaque at this (u, v)
  AmjuGL::Vert verts[4] =
  {
    AmjuGL::Vert(corners[0].x, corners[0].y, Z, u, v, 0, 1.0f, 0),
    AmjuGL::Vert(corners[1].x, corners[1].y, Z, u, v, 0, 1.0f, 0),
    AmjuGL::Vert(corners[2].x, corners[2].y, Z, u, v, 0, 1.0f, 0),
    AmjuGL::Vert(corners[3].x, corners[3].y, Z, u, v, 0, 1.0f, 0)
  };

  t[0].m_verts[0] = verts[0];
  t[0].m_verts[1] = verts[1];
  t[0].m_verts[2] = verts[2];

  t[1].m_verts[0] = verts[0];
  t[1].m_verts[1] = verts[2];
  t[1].m_verts[2] = verts[3];

  SetQuadColour(t, col);

  tris.push_back(t[0]);
  tris.push_back(t[1]);
}

void GuiMusicScore::BuildTriList()
{
  m_rect = EmptyRect();

#ifdef DEBUG_DRAW_GLYPH_RECTS
  glyphRects.clear();
#endif

  AmjuGL::Tris tris;

  for (const Glyph& g : m_glyphs)
  {
    if (g.m_char == QUAD_CHAR)
    {
      // Special case: make a quad from the 4 coords
      MakeQuad(g.m_corner, tris, g.m_colour);
    }
    else
    {
      AmjuGL::Tri t[2];

      m_atlas->SetSize(g.m_scale.x, g.m_scale.y);
      const bool NO_ITALIC = false;
#ifdef USE_BM_FONT
      m_atlas->MakeTris(g.m_char, 1.f, t, g.m_pos.x, g.m_pos.y, NO_ITALIC);
#else
      m_atlas->MakeTris(g.m_char - ' ', 1.f, t, g.m_pos.x, g.m_pos.y);
#endif

      SetQuadColour(t, g.m_colour);

      tris.push_back(t[0]);
      tris.push_back(t[1]);
    }

    // Keep track of bounding rect.
    // Add bounding box of the last two tris to the overall bounding rect
    Assert(tris.size() >= 2);
    AmjuGL::Vert verts[4] = 
    {
      tris[tris.size() - 2].m_verts[0],
      tris[tris.size() - 2].m_verts[1], // the other two verts are duplicates of these
      tris[tris.size() - 2].m_verts[2], // the other two verts are duplicates of these
      tris[tris.size() - 1].m_verts[2],
    };
    Rect r = EmptyRect();
    for (int i = 0; i < 4; i++)
    {
      r.SetIf(verts[i].m_x, verts[i].m_y);
    }

#ifdef DEBUG_DRAW_GLYPH_RECTS
    glyphRects.push_back(r);
#endif

    m_rect.Union(r);
  }
  m_triList = Amju::MakeTriList(tris);
}

#ifdef VERTEX_COLOUR_TEST
static float rnd()
{
  return (float)rand() / (float)RAND_MAX;
}
#endif // VERTEX_COLOUR_TEST
  
void GuiMusicScore::SetQuadColour(AmjuGL::Tri t[2], const Colour& col)
{
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      AmjuGL::Vert& v = t[i].m_verts[j];

      v.SetColour(col.m_r, col.m_g, col.m_b, col.m_a);

#ifdef VERTEX_COLOUR_TEST
      v.SetColour(rnd(), rnd(), rnd(), 1.0f); 
#endif // VERTEX_COLOUR_TEST
    }
  }
}

void GuiMusicScore::RefreshColours()
{
  // TODO TEMP TEST
  // We can do a lot better than this. We just need to update the colours
  //  on the verts for each glyph.
  BuildTriList();
}

int GuiMusicScore::GetNumGlyphs() const
{
  return m_glyphs.size();
}

GuiMusicScore::Glyph& GuiMusicScore::GetGlyph(int i)
{
  Assert(i < static_cast<int>(m_glyphs.size()));
  return m_glyphs[i];
}

Rect GuiMusicScore::CalcRect() const
{
  // Argh. We have to build the tri list to know how big the bounding box is.
  // TODO We can separate bbox and tri list.
  if (!m_triList)
  {
    const_cast<GuiMusicScore*>(this)->BuildTriList();
  }

  Rect r(m_rect);
  Vec2f pos = GetCombinedPos();
  Vec2f size = GetSize();
  r.Translate(pos.x, pos.y);
  r.Scale(size.x, size.y);
  return r;
}

}
