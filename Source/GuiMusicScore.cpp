// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <unordered_map>
#include <GuiFactory.h>
#include <StringUtils.h>
#include "GuiMusicScore.h"

namespace Amju
{
const char QUAD_CHAR = 0;

// This non-printable character is used for quads, which are a special case.
// Used for stave lines, bar lines, beams etc.
const char* QUAD_NAME = "quad";

static GuiElement* CreateMusicScore()
{
  return new GuiMusicScore;
}

GuiMusicScore::Glyph::Glyph(const Vec2f corner[4]) : m_char(QUAD_CHAR)
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
    { "stave", '=' },
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
    { "small-1", '�' },
    { "small-3", '�' },
    { "small-4", '�' },
    { "small-5", '�' },
    { "small-6", '�' },
    { "small-7", '�' },
    { "small-9", '�' },
    { "rest-crotchet", '�' },
    { "rest-3", '�' },
    { "ped", '�' },
    { "double-sharp", '�' },
    { "trill", '�' },
    { "tail", '�' }, // or bar line?
    { "tr", '�' },
    { "bracket-sharp", '�' },
    { "bracket-flat", '�' },
    { "bracket-natural", '�' },
    { "ff", '�' },
    { "pp", '�' },
    { "pppp", '�' },
    { "ppp", '�' },
    { "tail-down-4", '�' },
    { "tail-up-4", '�' },
    { "bracket-double-sharp", '�' },
    { "8vb", '�' },
    { "tail-down-3", '�' },
    { "tail-up-3", '�' },
    { "turn-inverted", '�' },

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
  m_atlas.Load("Fonts/Guido2/guido2-60pt.png", 16, 14, 1, 1);

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
  AmjuGL::SetColour(m_fgCol);
  AmjuGL::Translate(pos.x, pos.y, 0);
  AmjuGL::Scale(size.x, size.y, 1);
  AmjuGL::Draw(m_triList);
  AmjuGL::PopMatrix();
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
    f->ReportError("Expected gui rect colour");
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
      m_glyphs.push_back(Glyph(corners));
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
        m_glyphs.push_back(Glyph(ch, pos));
      }
      else
      {
        Vec2f scale(ToFloat(strs[3]), ToFloat(strs[4]));
        m_glyphs.push_back(Glyph(ch, pos, scale));
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

void GuiMusicScore::BuildTriList()
{
  AmjuGL::Tris tris;

  for (const Glyph& g : m_glyphs)
  {
    AmjuGL::Tri t[2];

    if (g.m_char == QUAD_CHAR)
    {
      // Special case: make a quad from the 4 coords
      const float Z = 0.5f;
      float u = .999f;
      float v = .001f; // TODO texture atlas should be opaque at this (u, v)
      AmjuGL::Vert verts[4] =
      {
        AmjuGL::Vert(g.m_corner[0].x, g.m_corner[0].y, Z, u, v, 0, 1.0f, 0),
        AmjuGL::Vert(g.m_corner[1].x, g.m_corner[1].y, Z, u, v, 0, 1.0f, 0),
        AmjuGL::Vert(g.m_corner[2].x, g.m_corner[2].y, Z, u, v, 0, 1.0f, 0),
        AmjuGL::Vert(g.m_corner[3].x, g.m_corner[3].y, Z, u, v, 0, 1.0f, 0)
      };

      t[0].m_verts[0] = verts[0];
      t[0].m_verts[1] = verts[1];
      t[0].m_verts[2] = verts[2];

      t[1].m_verts[0] = verts[0];
      t[1].m_verts[1] = verts[2];
      t[1].m_verts[2] = verts[3];
    }
    else
    {
      m_atlas.SetSize(g.m_scale.x, g.m_scale.y);
      m_atlas.MakeTris(g.m_char - ' ', 1.f, t, g.m_pos.x, g.m_pos.y);
    }
    tris.push_back(t[0]);
    tris.push_back(t[1]);
  }
  m_triList = Amju::MakeTriList(tris);
}

}


