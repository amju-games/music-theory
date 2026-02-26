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

