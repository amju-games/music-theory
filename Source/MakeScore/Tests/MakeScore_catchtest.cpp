// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman
//
// Unit tests for MakeScore
// 
// To build on Mac:
//
//  clang++ *.cpp ../*cpp -DCATCH -std=c++20
// 
// ..but there's a makefile and script to build and then run the tests.


#include "catch.hpp"
#include "MakeScore.h" 
#include "Suppress.h"

void MatchStart(const std::string& container, const std::string& containedAtStart)
{
  REQUIRE(container.size() >= containedAtStart.size());
  REQUIRE(container.substr(0, containedAtStart.size()) == containedAtStart);
}

TEST_CASE("treble-clef", "MakeScore")
{
  MakeScore ms("clef-t");
  ms.MakeInternal();
  const auto& strs = ms.GetOutputLines();
  MatchStart(strs[0], "stave");
  MatchStart(strs[1], "treble-clef");
}

TEST_CASE("crotchet, midi pitch", "MakeScore")
{
  SetSuppressFlags(SUPPRESS_ALL);
  MakeScore ms("clef-t <c> 60");
  ms.MakeInternal();
  const auto& strs = ms.GetOutputLines();
  MatchStart(strs[0], "stave");
  MatchStart(strs[1], "treble-clef");
  MatchStart(strs[2], "crotchet");
}

TEST_CASE("crotchet, musicxml pitch", "MakeScore")
{
  SetSuppressFlags(SUPPRESS_ALL);
  MakeScore ms("clef-t <c> c4");
  ms.MakeInternal();
  const auto& strs = ms.GetOutputLines();
  MatchStart(strs[0], "stave");
  MatchStart(strs[1], "treble-clef");
  MatchStart(strs[2], "crotchet");
}

TEST_CASE("pitch", "MakeScore")
{
  REQUIRE(GetPitch("60").m_midi == 60);
  REQUIRE(GetPitch("c4").m_midi == 60);
  REQUIRE(GetPitch("72").m_midi == 72);
  REQUIRE(GetPitch("c5").m_midi == 72);
  REQUIRE(GetPitch("c5+").m_midi == 73);
  REQUIRE(GetPitch("c5++").m_midi == 74);
  REQUIRE(GetPitch("d5-").m_midi == 73);
  REQUIRE(GetPitch("d5").m_midi == 74);
  REQUIRE(GetPitch("e5").m_midi == 76);
}

TEST_CASE("chord", "MakeScore")
{
  SetSuppressFlags(SUPPRESS_ALL);
  MakeScore ms("clef-t <c> (c4 e4)");
  ms.MakeInternal();
  const auto& strs = ms.GetOutputLines();
  MatchStart(strs[2], "crotchet");
}


