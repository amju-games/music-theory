// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <unordered_map>
#include <GuiFactory.h>
#include <StringUtils.h>
#include "GuiMusicScore.h"

namespace Amju
{
static GuiElement* CreateMusicScore()
{
  return new GuiMusicScore;
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
  m_atlas.Load("c:/Users/Jason/projects/music-theory/Assets/Fonts/Guido2/guido2-120pt.png", 16, 14, 1, 1);

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
    if (n == 3 || n == 5)
    {
      char ch;

      // TODO take care of special case names here, e.g. bars

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
      //f->ReportError("Unexpected format for music glyph: " + line);
      //return false;
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
    m_atlas.SetSize(g.m_scale.x, g.m_scale.y);
    m_atlas.MakeTris(g.m_char - ' ', 1.f, t, g.m_pos.x, g.m_pos.y);
    tris.push_back(t[0]);
    tris.push_back(t[1]);
  }
  m_triList = Amju::MakeTriList(tris);
}

}



