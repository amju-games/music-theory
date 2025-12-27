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

TEST_CASE("Build beat table", "GuiScrollScore")
{
  GuiScrollScore gss;
  gss.SetMinMaxTime(1.f, 2.f);
  gss.AddGlyph(MakeGlyph(3)); // x-coord
  gss.AddGlyph(MakeGlyph(2));
  gss.AddGlyph(MakeGlyph(4));

  gss.SetMinMaxTime(2.f, 3.f);
  gss.AddGlyph(MakeGlyph(8)); // x-coord
  gss.AddGlyph(MakeGlyph(7));
  gss.AddGlyph(MakeGlyph(9));

  BeatTable bt;
  gss.BuildBeatTable(bt);

/*
  for (const auto& [k, v] : bt)
  {
    std::cout << k << "\t" << v << "\n";
  }
*/

  // We want the MIN x-coord for each time. 
  REQUIRE(bt[1] == 2);
  REQUIRE(bt[2] == 7);
}

