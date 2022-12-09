// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <unordered_map>
#include <GuiFactory.h>
#include <StringUtils.h>
#include "GuiMusicScore.h"

namespace Amju
{
const char QUAD_CHAR = 0;
const char STAVE_CHAR = '=';

// This non-printable character is used for quads, which are a special case.
// Used for stave lines, bar lines, beams etc.
const char* QUAD_NAME = "quad";

static GuiElement* CreateMusicScore()
{
  return new GuiMusicScore;
}

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
static bool GlyphNameToCh(const std::string& s, char* ch)
{
  // Single character names just map to that character, e.g. a dot "." is character '.' in music score font.
  if (s.size() == 1)
  {
    *ch = s[0];
    return true;
  }

  static const std::unordered_map<std::string, char> NAMES = 
  {
    { "quad", QUAD_CHAR }, // special (non-printable) character to represent a quad
    { "triplet", '!' },
    { "sharp", '#' },
    { "segno", '%' },
    { "treble-clef", '&' },
    { "fff", '\'' },
    { "rest-5", '/' }, // come back to naming these
    { "stave", '=' }, // but stave is special case, we make from quads in BuildTriList
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
    { "bar-line", 'e' },
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
    { "small-1", '¡' },
    { "small-3", '£' },
    { "small-4", '¢' },
    { "small-5", '¦' },
    { "small-6", '§' },
    { "small-7", '¶' },
    { "small-9", 'ª' },
    { "rest-crotchet", '¥' },
    { "rest-3", '®' },
    { "ped", '°' },
    { "double-sharp", '´' },
    { "trill", 'µ' },
    { "tail", '·' }, // or bar line?
    { "tr", '¼' },
    { "bracket-sharp", '½' },
    { "bracket-flat", '¾' },
    { "bracket-natural", 'À' },
    { "ff", 'Á' },
    { "pp", 'Â' },
    { "pppp", 'Ã' },
    { "ppp", 'È' },
    { "tail-down-4", 'Ê' },
    { "tail-up-4", 'Ë' },
    { "bracket-double-sharp", 'Õ' },
    { "8vb", 'Ø' },
    { "tail-down-3", 'Ý' },
    { "tail-up-3", 'Þ' },
    { "turn-inverted", 'å' },

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

GuiMusicScore::GuiMusicScore()
{
  // Create texture atlas. TODO CONFIG
  m_atlas.Load("font2d/Guido2/guido2-60pt.png", 16, 14, 1, 1);

  m_fgCol = Colour(0, 0, 0, 1); // default to black
}

void GuiMusicScore::AddToFactory()
{
  TheGuiFactory::Instance()->Add("music-score", CreateMusicScore);
}

void GuiMusicScore::Draw()
{
  if (!m_triList)
  {
    BuildTriList();
  }

  Vec2f pos = GetCombinedPos();
  Vec2f size = GetSize();

  m_atlas.Bind();
  AmjuGL::PushMatrix();
  // Use vertex colours 
#ifdef WIN32
  // Desktop GL: need shader to combine global and vertex colours
  AmjuGL::SetColour(m_fgCol);
#endif
  AmjuGL::Translate(pos.x, pos.y, 0);
  AmjuGL::Scale(size.x, size.y, 1);
  AmjuGL::Draw(m_triList);
  AmjuGL::PopMatrix();
}

void GuiMusicScore::AddGlyph(const Glyph& g)
{
  m_glyphs.push_back(g);
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
    // Split line. Format OK? Has optional scale?
    Strings strs = Split(line, ',');
    int n = strs.size();
    // Quads are a special case
    if (n == 9 && strs[0] == QUAD_NAME)
    {
      // 4 corners
      Vec2f corners[4] = 
      {
        Vec2f(ToFloat(strs[1]), ToFloat(strs[2])),
        Vec2f(ToFloat(strs[3]), ToFloat(strs[4])),
        Vec2f(ToFloat(strs[5]), ToFloat(strs[6])),
        Vec2f(ToFloat(strs[7]), ToFloat(strs[8]))
      };
      m_glyphs.push_back(Glyph(corners, m_fgCol));
    }
    else if (n == 3 || n == 5)
    {
      char ch;

      if (!GlyphNameToCh(strs[0], &ch))
      {
        f->ReportError("Unexpected music glyph name: " + strs[0]);
        return false;
      }
      Vec2f pos(ToFloat(strs[1]), ToFloat(strs[2]));
      if (n == 3)
      {
        m_glyphs.push_back(Glyph(ch, pos, m_fgCol));
      }
      else
      {
        Vec2f scale(ToFloat(strs[3]), ToFloat(strs[4]));
        m_glyphs.push_back(Glyph(ch, pos, scale, m_fgCol));
      }
    }
    else
    {
      break;
    }
  }
  return true;
}

void GuiMusicScore::SetFgCol(const Colour& col)
{
  m_fgCol = col;
}

void GuiMusicScore::MakeQuad(const Vec2f corners[4], AmjuGL::Tris& tris, const Colour& col)
{
  AmjuGL::Tri t[2];

  const float Z = 0.5f;
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

void GuiMusicScore::MakeStave(const Glyph& g, AmjuGL::Tris& tris, const Colour& col)
{
  const float Y_OFFSET = 4.75f;
  float h = g.m_scale.y * 0.01f;
  float w = g.m_scale.x;
  // Make stave from 5 quads
  for (int i = 0; i < 5; i++)
  {
    float y = g.m_pos.y + (static_cast<float>(i) + Y_OFFSET) * 0.1f * g.m_scale.y;
    Vec2f corners[4] =
    {
      Vec2f(g.m_pos.x,     y),
      Vec2f(g.m_pos.x + w, y),
      Vec2f(g.m_pos.x + w, y + h),
      Vec2f(g.m_pos.x,     y + h),
    };

    MakeQuad(corners, tris, col);
  }
}

void GuiMusicScore::BuildTriList()
{
  AmjuGL::Tris tris;

  for (const Glyph& g : m_glyphs)
  {
    if (g.m_char == STAVE_CHAR)
    {
      MakeStave(g, tris, g.m_colour);
    }
    else if (g.m_char == QUAD_CHAR)
    {
      // Special case: make a quad from the 4 coords
      MakeQuad(g.m_corner, tris, g.m_colour);
    }
    else
    {
      AmjuGL::Tri t[2];

      m_atlas.SetSize(g.m_scale.x, g.m_scale.y);
      m_atlas.MakeTris(g.m_char - ' ', 1.f, t, g.m_pos.x, g.m_pos.y);

      SetQuadColour(t, g.m_colour);

      tris.push_back(t[0]);
      tris.push_back(t[1]);
    }
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

}



