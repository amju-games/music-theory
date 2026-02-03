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
  MatchStart(strs[2], "note-solid"); // crotchet note head
}

TEST_CASE("minim, midi pitch", "MakeScore")
{
  SetSuppressFlags(SUPPRESS_ALL);
  MakeScore ms("clef-t <m> 62");
  ms.MakeInternal();
  const auto& strs = ms.GetOutputLines();
  MatchStart(strs[0], "stave");
  MatchStart(strs[1], "treble-clef");
  MatchStart(strs[2], "note-minim"); // minim note head
}

TEST_CASE("crotchet, musicxml pitch", "MakeScore")
{
  SetSuppressFlags(SUPPRESS_ALL);
  MakeScore ms("clef-t <c> c4");
  ms.MakeInternal();
  const auto& strs = ms.GetOutputLines();
  MatchStart(strs[0], "stave");
  MatchStart(strs[1], "treble-clef");
  MatchStart(strs[2], "note-solid");
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

TEST_CASE("Chord detection", "[.]")
{
  SetSuppressFlags(SUPPRESS_ALL);
  MakeScore ms("clef-t <c> (c4 e4)");
  ms.MakeInternal();
  const auto& strs = ms.GetOutputLines();
  MatchStart(strs[2], "crotchet");
  REQUIRE(0);
}

TEST_CASE("PreprocessTokens", "[MakeScore]")
{
  // Test that preprocessing removes tokens from within chord markers

  MakeScore ms;

  {
  // ff direction within chord, at end
  std::vector<std::string> tokens = { "(", "c4", "c", "ff", ")", "d4" };
  const std::vector<std::string> expected = { "(", "c4", "c", ")", "ff", "d4" };
  
  ms.PreprocessTokens(tokens);
  REQUIRE(tokens == expected);
  }

  {
  // ff direction within chord, at start
  std::vector<std::string> tokens = { "(", "ff", "c4", "c", ")", "d4" };
  const std::vector<std::string> expected = { "(", "c4", "c", ")", "ff", "d4" };
  
  ms.PreprocessTokens(tokens);
  REQUIRE(tokens == expected);
  }

  {
  // Multi directions within chord, at start
  std::vector<std::string> tokens = { "(", "ff", "mp", "c", ")", "d4" };
  const std::vector<std::string> expected = { "(", "c", ")", "ff", "mp", "d4" };
  
  ms.PreprocessTokens(tokens);
  REQUIRE(tokens == expected);
  }

  {
  // Multi directions within chord, split up 
  std::vector<std::string> tokens = { "(", "ff", "c4", "mp", ")", "d4" };
  const std::vector<std::string> expected = { "(", "c4", ")", "ff", "mp", "d4" };
  
  ms.PreprocessTokens(tokens);
  REQUIRE(tokens == expected);
  }

  {
  // BIG test, real world data
  std::vector<std::string> tokens = 
  {
    "(", "<c>", "72", "mf", "48", ")", "(", "72", "60", ")", 
    " (", " 79", " 64", " )", " (", "79", " mp", " 60", " )", " |", 
    " (", " 81", " 65", " )", " (", " 81", " 60", " )", 
    " (", " <m>", " 79", " ff", " <c>", " 64", " )" 
  };

  const std::vector<std::string> expected = 
  {
    "(", "<c>", "72", "48", ")", "mf", "(", "72", "60", ")", 
    "(", "79", "64", ")", "(", "79", "60", ")", "mp", "|", 
    "(", "81", "65", ")", "(", "81", "60", ")", 
    "(", "<m>", "79", "<c>", "64", ")", "ff" 
  };
  
  ms.PreprocessTokens(tokens);
  REQUIRE(tokens == expected);
  }
}

