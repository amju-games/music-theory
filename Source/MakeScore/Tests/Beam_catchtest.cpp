#include <iostream>
#include "catch.hpp"
#include "Bar.h"
#include "BeamGroup.h"
#include "Pitch.h"

// NB
// Bar::AddNote(type/duration token, pitch, flags, start time)

TEST_CASE("Find beam groups in bar 1", "[Beam]")
{
  Bar bar;

  //  c  q--q
  bar.AddNote("c", Pitch(60), 0, 0);
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_CROTCHET);
  bar.AddNote("q", Pitch(62), 0, TIMEVAL_CROTCHET + TIMEVAL_QUAVER);

  auto beamGroups = FindBeamGroups(bar.GetGlyphs());
  REQUIRE(beamGroups.size() == 1);
}

TEST_CASE("Find beam groups in bar 2", "[Beam]")
{
  Bar bar;

  //  c  q--q  c
  bar.AddNote("c", Pitch(60), 0, 0);
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_CROTCHET);
  bar.AddNote("q", Pitch(62), 0, TIMEVAL_CROTCHET + TIMEVAL_QUAVER);
  bar.AddNote("c", Pitch(63), 0, TIMEVAL_CROTCHET * 2);

  auto beamGroups = FindBeamGroups(bar.GetGlyphs());
  REQUIRE(beamGroups.size() == 1);
}

TEST_CASE("Find beam groups in bar 3", "[Beam]")
{
  Bar bar;

  //  c q--q c q--q--q
  bar.AddNote("c", Pitch(60), 0, 0);
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_CROTCHET);
  bar.AddNote("q", Pitch(62), 0, TIMEVAL_CROTCHET + TIMEVAL_QUAVER);
  bar.AddNote("c", Pitch(63), 0, TIMEVAL_CROTCHET * 2);
  bar.AddNote("q", Pitch(64), 0, TIMEVAL_CROTCHET * 3);
  bar.AddNote("q", Pitch(65), 0, TIMEVAL_CROTCHET * 3 + TIMEVAL_QUAVER);
  bar.AddNote("q", Pitch(66), 0, TIMEVAL_CROTCHET * 4);

  auto beamGroups = FindBeamGroups(bar.GetGlyphs());
  REQUIRE(beamGroups.size() == 2);
}

TEST_CASE("Find beam groups in bar 4", "[Beam]")
{
  Bar bar;

  //  c  q  // single q => no beam group
  bar.AddNote("c", Pitch(60), 0, 0);
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_CROTCHET);

  auto beamGroups = FindBeamGroups(bar.GetGlyphs());
  REQUIRE(beamGroups.size() == 0);
}

TEST_CASE("Find beam groups in bar 5", "[Beam]")
{
  Bar bar;

  //  c  q  c // single q => no beam group
  bar.AddNote("c", Pitch(60), 0, 0);
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_CROTCHET);
  bar.AddNote("c", Pitch(62), 0, TIMEVAL_CROTCHET + TIMEVAL_QUAVER);

  auto beamGroups = FindBeamGroups(bar.GetGlyphs());
  REQUIRE(beamGroups.size() == 0);
}

TEST_CASE("Find beam groups in bar 6", "[Beam]")
{
  Bar bar;

  //  q--q--q   => all one beam group
  bar.AddNote("q", Pitch(60), 0, 0);
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_QUAVER);
  bar.AddNote("q", Pitch(62), 0, TIMEVAL_CROTCHET);

  auto beamGroups = FindBeamGroups(bar.GetGlyphs());
  REQUIRE(beamGroups.size() == 1);
}

TEST_CASE("Find beam levels 1", "[Beam]")
{
  Bar bar;

  //  q--q
  bar.AddNote("q", Pitch(61), 0, 0);
  bar.AddNote("q", Pitch(62), 0, TIMEVAL_QUAVER);

  auto beamGroups = FindBeamGroups(bar.GetGlyphs());
  REQUIRE(beamGroups.size() == 1);
  const BeamGroup& bg = beamGroups[0];
  REQUIRE(bg.GetNumMembers() == 2);

// ??
//  REQUIRE(bg.GetFlagLevel(0) == 0);
//  REQUIRE(bg.GetFlagLevel(1) == 0);
  
}

TEST_CASE("Decide stem direction 1", "[Beam]")
{
  Bar bar;
  bar.SetClef(Clef::CLEF_TREBLE);

  //  c q--q c q--q--q
  bar.AddNote("c", Pitch(60), 0, 0);
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_CROTCHET);
  bar.AddNote("q", Pitch(62), 0, TIMEVAL_CROTCHET + TIMEVAL_QUAVER);
  bar.AddNote("c", Pitch(63), 0, TIMEVAL_CROTCHET * 2);

  // High notes
  bar.AddNote("q", Pitch(74), 0, TIMEVAL_CROTCHET * 3);
  bar.AddNote("q", Pitch(60), 0, TIMEVAL_CROTCHET * 3 + TIMEVAL_QUAVER);
  bar.AddNote("q", Pitch(76), 0, TIMEVAL_CROTCHET * 4);

  auto beamGroups = FindBeamGroups(bar.GetGlyphs());
  REQUIRE(beamGroups.size() == 2);

  beamGroups[0].DecideStemDirections(bar.GetGlyphs());
  beamGroups[1].DecideStemDirections(bar.GetGlyphs());

  // Low notes, stems go up.
  REQUIRE(beamGroups[0].GetStemDirection() == StemDir::UP);

  // Majority of high notes, stems go down. 
  REQUIRE(beamGroups[1].GetStemDirection() == StemDir::DOWN);
}

static void SetXCoordsOfGlyphs(Bar& bar)
{
  auto& glyphs = bar.GetGlyphs();
  float x = 0;
  for (auto& g : glyphs)
  {
    g->SetX(x);
    x += 1.f;
  }
}

TEST_CASE("Calc primary beam y coords at ends", "[Beam]")
{
  Bar bar;

  //  q--q--q   => three quavers in one beam group

  // AddNote params: type ("q", "qq", "qqq"), pitch, unused, start time
  bar.AddNote("q", Pitch(60), 0, 0); 
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_QUAVER);
  bar.AddNote("q", Pitch(62), 0, TIMEVAL_CROTCHET);
  SetXCoordsOfGlyphs(bar);

  auto beamGroups = FindBeamGroups(bar.GetGlyphs());
  REQUIRE(beamGroups.size() == 1);
  auto& bg = beamGroups[0];

  bg.DecideStemDirections(bar.GetGlyphs());
  // Get the coords of the ends of the primary beam in stave lines:
  //  this is more testable than floats.
  const auto [y1, y2] = bg.CalcYStaveLinesAtEnds(bar.GetGlyphs());

  REQUIRE(y1 == 2); // Middle C is stave line -2, plus height 4
  REQUIRE(y2 == 3); // 62 is d4, stave line -1, plus height 4 
}

TEST_CASE("Calc primary beam y coords at ends - Advanced", "[Beam]") 
{
  // Gemini-generated tests
  Bar bar;

  SECTION("Concave group - Middle note forces lift") {
    // Three notes: c4, g4, d4. All stems UP.
    // c4 (line -2), g4 (line 2), d4 (line -1)
    // The middle note (g4) is much higher and will "poke" through 
    // a beam drawn strictly between c4 and d4.
    bar.AddNote("q", Pitch(60), 0, 0); 
    bar.AddNote("q", Pitch(67), 0, TIMEVAL_QUAVER);
    bar.AddNote("q", Pitch(62), 0, TIMEVAL_CROTCHET);
    SetXCoordsOfGlyphs(bar);

    auto beamGroups = FindBeamGroups(bar.GetGlyphs());
    auto& bg = beamGroups[0];
    bg.SetStemDirection(StemDir::UP); // Force UP for test

    const auto [y1, y2] = bg.CalcYStaveLinesAtEnds(bar.GetGlyphs());

    // Middle note g4 is at stave line 2. 
    // Min stem 4 means beam must be at least y=6 at the middle.
    // Because the ends are c4(-2) and d4(-1), the beam is nearly flat.
    // To clear the middle g4, y1 and y2 must both be pushed up.
    REQUIRE(y1 >= 6); 
    REQUIRE(y2 >= 6);
  }

  SECTION("Semiquaver stack - Secondary beam requires more room") {
    // Two semiquavers (16th notes) at c4.
    // Level 2 means we need to clear the primary AND secondary beam.
    bar.AddNote("qq", Pitch(60), 0, 0);
    bar.AddNote("qq", Pitch(60), 0, TIMEVAL_SEMIQUAVER);
    SetXCoordsOfGlyphs(bar);

    auto beamGroups = FindBeamGroups(bar.GetGlyphs());
    auto& bg = beamGroups[0];
    bg.SetStemDirection(StemDir::UP);

    const auto [y1, y2] = bg.CalcYStaveLinesAtEnds(bar.GetGlyphs());

    // c4 is -2. 
    // Quaver (Level 1) would be -2 + 4 = 2.
    // Semiquaver (Level 2) needs -2 + 4 + 1.0 (stack) = 3.
    REQUIRE(y1 == 3);
    REQUIRE(y2 == 3);
  }
  
  SECTION("Descending group with StemDir DOWN") {
    // Higher notes: c5(stave line 5) to g4(2). Stems DOWN.
    // Expected y: c5(5-4) = 1, g4(2-4) = -2
    bar.AddNote("q", Pitch(72), 0, 0);
    bar.AddNote("q", Pitch(67), 0, TIMEVAL_QUAVER);
    SetXCoordsOfGlyphs(bar);
    
    auto beamGroups = FindBeamGroups(bar.GetGlyphs());
    auto& bg = beamGroups[0];
    bg.SetStemDirection(StemDir::DOWN);
    
    const auto [y1, y2] = bg.CalcYStaveLinesAtEnds(bar.GetGlyphs());
    REQUIRE(y1 == 1);
    REQUIRE(y2 == -2);
  }
}

TEST_CASE("Calc vertical dist from point to line seg", "[Beam]")
{
  //    |
  // 3  x--------x  <--- horizontal beam
  // 2  |
  // 1  |
  // 0  +--p------- <--- point -- so vertical dist is 3.
  //    0  1  2  3
  vec2 p(1.f, 0.f); // point 
  std::pair<vec2, vec2> beam { {0.f, 3.f}, {3.f, 3.f} };
  REQUIRE(CalcStaveLineToBeamDistance(p, beam) == 3.f);
}

TEST_CASE("Calc vertical dist from point to line seg 2", "[Beam]") 
{
  // Gemini-generated tests
    SECTION("Horizontal beam") {
        vec2 p(1.f, 0.f);
        std::pair<vec2, vec2> beam{ {0.f, 3.f}, {3.f, 3.f} };
        REQUIRE(CalcStaveLineToBeamDistance(p, beam) == 3.f);
    }

    SECTION("Sloped beam") {
        // Beam goes from (0,0) to (2,2). At x=1, y should be 1.
        // Point is at (1, 0), so distance should be 1.
        vec2 p(1.f, 0.f);
        std::pair<vec2, vec2> beam{ {0.f, 0.f}, {2.f, 2.f} };
        REQUIRE(CalcStaveLineToBeamDistance(p, beam) == 1.f);
    }

    SECTION("Beam defined right-to-left") {
        // Same as sloped beam, but swap start/end points
        vec2 p(1.f, 0.f);
        std::pair<vec2, vec2> beam{ {2.f, 2.f}, {0.f, 0.f} };
        REQUIRE(CalcStaveLineToBeamDistance(p, beam) == 1.f);
    }

    SECTION("Point above the beam") {
        // Beam at y=1, point at y=5
        vec2 p(1.f, 5.f);
        std::pair<vec2, vec2> beam{ {0.f, 1.f}, {2.f, 1.f} };
        REQUIRE(CalcStaveLineToBeamDistance(p, beam) == 4.f);
    }

    SECTION("Point outside horizontal bounds") {
        vec2 p(5.f, 0.f);
        std::pair<vec2, vec2> beam{ {0.f, 1.f}, {2.f, 1.f} };
        // We defined this to return -1.0f for "out of bounds"
        REQUIRE(CalcStaveLineToBeamDistance(p, beam) == -1.0f);
    }
}


