#include <iostream>
#include "catch.hpp"
#include "Bar.h"
#include "BeamGroup.h"
#include "Pitch.h"

// NB
// Bar::AddNote(type/duration token, pitch, flags, start time)

// TODO This isn't getting used if REQUIRE fails comparing pairs of ints
std::ostream& operator<<(std::ostream& os, const std::pair<int, int> p)
{
  return os << "(" << p.first << ", " << p.second << ")";
}

TEST_CASE("Find beam groups in bar 1", "[Beam]")
{
  Bar bar;

  //  c  q--q
  bar.AddNote("c", Pitch(60), 0, 0);
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_CROTCHET);
  bar.AddNote("q", Pitch(62), 0, TIMEVAL_CROTCHET + TIMEVAL_QUAVER);

  auto beamGroups = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_NONE);
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

  auto beamGroups = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_NONE);
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

  auto beamGroups = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_NONE);
  REQUIRE(beamGroups.size() == 2);
}

TEST_CASE("Find beam groups in bar 4", "[Beam]")
{
  Bar bar;

  //  c  q  // single q => no beam group
  bar.AddNote("c", Pitch(60), 0, 0);
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_CROTCHET);

  auto beamGroups = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_NONE);
  REQUIRE(beamGroups.size() == 0);
}

TEST_CASE("Find beam groups in bar 5", "[Beam]")
{
  Bar bar;

  //  c  q  c // single q => no beam group
  bar.AddNote("c", Pitch(60), 0, 0);
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_CROTCHET);
  bar.AddNote("c", Pitch(62), 0, TIMEVAL_CROTCHET + TIMEVAL_QUAVER);

  auto beamGroups = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_NONE);
  REQUIRE(beamGroups.size() == 0);
}

TEST_CASE("Find beam groups in bar 6", "[Beam]")
{
  Bar bar;

  //  q--q--q   => all one beam group
  bar.AddNote("q", Pitch(60), 0, 0);
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_QUAVER);
  bar.AddNote("q", Pitch(62), 0, TIMEVAL_CROTCHET);

  auto beamGroups = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_NONE);
  REQUIRE(beamGroups.size() == 1);
}

TEST_CASE("Find beam levels 1", "[Beam]")
{
  Bar bar;

  //  q--q
  bar.AddNote("q", Pitch(61), 0, 0);
  bar.AddNote("q", Pitch(62), 0, TIMEVAL_QUAVER);

  auto beamGroups = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_NONE);
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

  auto beamGroups = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_NONE);
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

  auto beamGroups = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_NONE);
  REQUIRE(beamGroups.size() == 1);
  auto& bg = beamGroups[0];

  bg.DecideStemDirections(bar.GetGlyphs());
  // Get the coords of the ends of the primary beam in stave lines:
  //  this is more testable than floats.
  const auto [y1, y2] = bg.CalcYStaveLinesAtEnds(bar.GetGlyphs());

  REQUIRE(y1 == 4); // Middle C is stave line -2, plus height 4
  REQUIRE(y2 == 5); // 62 is d4, stave line -1, plus height 4 
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

    auto beamGroups = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_NONE);
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

    auto beamGroups = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_NONE);
    auto& bg = beamGroups[0];
    bg.SetStemDirection(StemDir::UP);

    const auto [y1, y2] = bg.CalcYStaveLinesAtEnds(bar.GetGlyphs());

    // c4 is -2. 
    // Quaver (Level 1) would be -2 + 4 = 2.
    // Semiquaver (Level 2) needs -2 + 4 + 1.0 (stack) = 3.
    // But min height is 6.
    REQUIRE(y1 == 6);
    REQUIRE(y2 == 6);
  }
  
  SECTION("Descending group with StemDir DOWN") {
    // Higher notes: c5(stave line 5) to g4(2). Stems DOWN.
    // Expected y: c5(5-4) = 1, g4(2-4) = -2
    bar.AddNote("q", Pitch(72), 0, 0);
    bar.AddNote("q", Pitch(67), 0, TIMEVAL_QUAVER);
    SetXCoordsOfGlyphs(bar);
    
    auto beamGroups = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_NONE);
    auto& bg = beamGroups[0];
    bg.SetStemDirection(StemDir::DOWN);
    
    const auto [y1, y2] = bg.CalcYStaveLinesAtEnds(bar.GetGlyphs());
    REQUIRE(y1 == -2);
    REQUIRE(y2 == -4);
  }
}

TEST_CASE("Broken beam stub direction", "[Beam]") 
{
  Bar bar;

  // Scenario: Dotted Eighth (Level 1) followed by Sixteenth (Level 2)
  // The 16th note's level-2 stub MUST point LEFT.
  bar.AddNote("q.", Pitch(60), 0, 0);                  // Dotted Quaver (8th)
  bar.AddNote("qq", Pitch(62), 0, TIMEVAL_SEMIQUAVER); // Semiquaver (16th)

  auto beamGroups = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_NONE);
  auto& bg = beamGroups[0];
  
  // Let's imagine a function: 
  // float GetStubX(int glyphIndex, int level, float stubLength)
  
  SECTION("Sixteenth at end points left") 
  {
    float x_16th = bar.GetGlyphs()[1]->GetPos().x;
    float stubLength = 1.0f;
    
    // This should return x_16th - stubLength because it's at the end
    float endX = bg.CalcStubEndPosX(1, 2, stubLength, bar.GetGlyphs());
    
    REQUIRE(endX < x_16th); 
    REQUIRE(endX == Approx(x_16th - stubLength));
  }
}

TEST_CASE("Find split beam groups 1", "[Beam]")
{
  Bar bar;

  //  q--q--q--q   => all one beam group, split by middle beat
  bar.AddNote("q", Pitch(60), 0, 0);
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_QUAVER);
  bar.AddNote("q", Pitch(62), 0, TIMEVAL_QUAVER * 2);
  bar.AddNote("q", Pitch(63), 0, TIMEVAL_QUAVER * 3);

  const auto bgs = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_TWO_FOUR);
  REQUIRE(bgs.size() == 2);
  REQUIRE(bgs[0].GetRange() == std::make_pair(0, 2));
  REQUIRE(bgs[1].GetRange() == std::make_pair(2, 4));
}

TEST_CASE("Find split beam groups 2", "[Beam]")
{
  Bar bar;

  //  q--q--q r  => q--q q r 
  bar.AddNote("q", Pitch(60), 0, 0);
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_QUAVER);
  bar.AddNote("q", Pitch(62), 0, TIMEVAL_QUAVER * 2);
  bar.AddRest("q", 0, TIMEVAL_QUAVER * 3, false);

  const auto bgs = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_TWO_FOUR);
  REQUIRE(bgs.size() == 1);
  REQUIRE(bgs[0].GetRange() == std::make_pair(0, 2));
}

TEST_CASE("Find split beam groups 3", "[Beam]")
{
  Bar bar;

  //  r q--q--q  => r q q--q 
  bar.AddRest("q", 0, 0, false);
  bar.AddNote("q", Pitch(60), 0, TIMEVAL_QUAVER);
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_QUAVER * 2);
  bar.AddNote("q", Pitch(62), 0, TIMEVAL_QUAVER * 3);

  const auto bgs = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_TWO_FOUR);
  REQUIRE(bgs.size() == 1);
  REQUIRE(bgs[0].GetRange() == std::make_pair(2, 4));
}

TEST_CASE("Find split beam groups 4", "[Beam]")
{
  Bar bar;

  //  4/4 q--q--q--q--q--q--q--q   => all one beam group, split by middle beat
  bar.AddNote("q", Pitch(60), 0, 0);
  bar.AddNote("q", Pitch(61), 0, TIMEVAL_QUAVER);
  bar.AddNote("q", Pitch(62), 0, TIMEVAL_QUAVER * 2);
  bar.AddNote("q", Pitch(63), 0, TIMEVAL_QUAVER * 3);
  bar.AddNote("q", Pitch(64), 0, TIMEVAL_QUAVER * 4);
  bar.AddNote("q", Pitch(65), 0, TIMEVAL_QUAVER * 5);
  bar.AddNote("q", Pitch(66), 0, TIMEVAL_QUAVER * 6);
  bar.AddNote("q", Pitch(67), 0, TIMEVAL_QUAVER * 7);

  const auto bgs = FindBeamGroups(bar.GetGlyphs(), 0, TimeSig::TIME_SIG_FOUR_FOUR);
  REQUIRE(bgs.size() == 2);
  REQUIRE(bgs[0].GetRange() == std::make_pair(0, 4));
  REQUIRE(bgs[1].GetRange() == std::make_pair(4, 8));
}

TEST_CASE("IsNoteOnBeamBreak rules", "[Beam]") 
{
  TimeValue barStart = 0.0f;

  SECTION("Four-Four (4/4) Rules") 
  {
    // Beat 3 (Time 2.0) is a major break point in 4/4
    REQUIRE(IsNoteOnBeamBreak(2.0f, barStart, TimeSig::TIME_SIG_FOUR_FOUR) == true);
    // Beat 2 (Time 1.0) is NOT a major break (usually grouped 1+2, 3+4)
    REQUIRE(IsNoteOnBeamBreak(1.0f, barStart, TimeSig::TIME_SIG_FOUR_FOUR) == false);
  }

  SECTION("Three-Four (3/4) Rules") 
  {
    // In 3/4, we usually don't break the primary beam between beats 
    // unless the notes are very short, but beat 2 and 3 are technically "breaks" 
    // if we want to show every crotchet.
    REQUIRE(IsNoteOnBeamBreak(1.0f, barStart, TimeSig::TIME_SIG_THREE_FOUR) == true);
    REQUIRE(IsNoteOnBeamBreak(2.0f, barStart, TimeSig::TIME_SIG_THREE_FOUR) == true);
  }

  SECTION("Compound Time (6/8)") 
  {
    // 6/8 is grouped in two beats of 1.5 crotchets each.
    // Break should happen at Time 1.5
    REQUIRE(IsNoteOnBeamBreak(1.5f, barStart, TimeSig::TIME_SIG_SIX_EIGHT) == true);
    REQUIRE(IsNoteOnBeamBreak(0.5f, barStart, TimeSig::TIME_SIG_SIX_EIGHT) == false);
  }
}
