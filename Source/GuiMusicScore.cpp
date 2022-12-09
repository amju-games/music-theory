// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <map>
#include <StringUtils.h>
#include "GuiMusicScore.h"

namespace Amju
{
// Look up character in font from human-readable name
static bool GlyphNameToCh(const std::string& s, char* ch)
{
  static const std::map<std::string, char> NAMES = 
  {
    { "treb", '&' },
    { "stave", '=' },
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
  m_atlas.Load("Fonts/Guido2/guido2-120pt.png", 16, 14, 1, 1);
}

void GuiMusicScore::Draw()
{
  if (!m_triList)
  {
    BuildTriList();
  }

  Vec2f pos = GetCombinedPos();
 
  m_atlas.Bind();
  AmjuGL::PushMatrix();
  AmjuGL::Translate(pos.x, pos.y, 0);
  AmjuGL::Draw(m_triList);
  AmjuGL::PopMatrix();
}

bool GuiMusicScore::Load(File* f)
{
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
    if (n == 3 || n == 5)
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
      f->ReportError("Unexpected format for music glyph: " + line);
      return false;
    }
  }
  return true;
}

void GuiMusicScore::SetFgCol(const Colour& col)
{
}

void GuiMusicScore::BuildTriList()
{
  AmjuGL::Tris tris;

  for (const Glyph& g : m_glyphs)
  {
    AmjuGL::Tri t[2];
    m_atlas.MakeTris(g.m_char - ' ', 1.f, t, g.m_pos.x, g.m_pos.y);
    tris.push_back(t[0]);
    tris.push_back(t[1]);

  }
  m_triList = Amju::MakeTriList(tris);

//  m_triList = m_atlas.MakeTriList(0, 0, str.c_str(), m_scaleX);
}

}



