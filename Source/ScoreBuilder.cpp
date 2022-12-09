// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <StringUtils.h>
#include "GuiMusicScore.h"
#include "ScoreBuilder.h"

namespace Amju
{
void ScoreBuilder::Write(GuiMusicScore& gui)
{
  // Write each bar
  for (const Bar& b : m_bars)
  {
    WriteBar(b, gui);
  }
}

void ScoreBuilder::WriteBar(const ScoreBuilder::Bar& b, GuiMusicScore& gui)
{
  for (const Note& n : b)
  {
    // Choose symbol from duration. 
    // Choose y-position from pitch, current clef.
    // Add ledger lines if off stave.

    Vec2f scale(1, 1); // except for small symbols, e.g. clef change

    // Glyphs are usually black
    Colour colour(0, 0, 0, 1);

    const float HEIGHT_SCALE = 0.05f;
    float x = static_cast<float>(n.m_stavePosX) / 255.0f; // TODO
    float y = static_cast<float>(n.m_stavePosY + 4) * HEIGHT_SCALE;
    Vec2f pos(x, y);
    char ch = 'w'; // TODO 
    GuiMusicScore::Glyph g(ch, pos, scale, colour);
    gui.AddGlyph(g);

    // Add ledger lines
    for (int i = 5; i < n.m_stavePosY; i += 2)
    {
      gui.AddGlyph(GuiMusicScore::Glyph('_', Vec2f(x, static_cast<float>(i + 5) * HEIGHT_SCALE), scale, colour));
    }

    for (int i = -6; i >= n.m_stavePosY; i -= 2)
    {
      gui.AddGlyph(GuiMusicScore::Glyph('_', Vec2f(x, static_cast<float>(i + 4)* HEIGHT_SCALE), scale, colour));
    }

    x += 0.5f;
  }
  gui.BuildTriList();
}

void ScoreBuilder::Add(const Note& note, int stave)
{
  int bar = note.m_bar;
  AddBar(bar); // make sure bar exists

  m_bars[bar - 1].push_back(note); // bar num is 1-based
}

void ScoreBuilder::AddBar(int barNum1Based)
{
  // Resize bar vectors 
  m_bars.resize(barNum1Based);
}

// Function called to parse a section in a Score Bui8lder string, e.g. "bar-time=128".
// First arg is the string on the rhs of the equals sign.
using ScoreParseFunc = void(*)(const std::string&, ScoreBuilder*);

void ParseBarTime(const std::string& s, ScoreBuilder* sb)
{
}

void ParseSymbol(const std::string& s, ScoreBuilder* sb)
{
}

static const std::map<std::string, ScoreParseFunc> PARSE_FUNCTION_MAP = 
{
  { "bar-time", ParseBarTime },
  { "symbol", ParseSymbol }
};

bool ScoreBuilder::SetFromString(const std::string& scoreStr, std::string* error)
{
  Strings strs = Split(scoreStr, ';');
  for (const std::string& section : strs)
  {
    // Each section has form "a=b" ?
    Strings eq = Split(section, '=');
    if (eq.size() == 2)
    {
      // Section is in form "a=b"
      // Look up parse function for lhs of section
      auto it = PARSE_FUNCTION_MAP.find(eq[0]);
      if (it == PARSE_FUNCTION_MAP.end())
      {
        *error = "Unexpected section: " + eq[0];
        return false;
      }
      // Call function we looked up
      ScoreParseFunc fn = it->second;
      Assert(fn);
      fn(eq[1], this);
    }
  }
  return true;
}
}
