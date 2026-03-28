#include "catch.hpp"
#include "GuiScrollScore.h"

using namespace Amju;
 
auto MakeGlyph(int x)
{
  GuiMusicScore::Glyph g;
  g.m_char = 1;
  g.m_pos = { static_cast<float>(x), 0.f };
  return g;
}

