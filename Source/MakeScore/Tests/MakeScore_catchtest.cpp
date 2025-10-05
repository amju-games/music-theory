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
#include "../MakeScore.h" 

TEST_CASE("Test MakeScore Broken", "MakeScore")
{
  MakeScore ms("c"); // one bar of one crotchet
  ms.MakeInternal();
  REQUIRE(ms.ToString() == "crotchet");
}

TEST_CASE("Load sample music xml file", "MakeScore")
{
  // test1 has 
  //  - one part
  //  - one stave
  //  - one bar
  //    -  treble clef, key of c major, 4/4, one semibreve middle C.

  MakeScore ms;
  bool successfulLoad = ms.LoadXml("test1.musicxml");
  REQUIRE(successfulLoad);
  REQUIRE(ms.NumParts() == 1);
  REQUIRE(ms.NumBars() == 1);
}


TEST_CASE("Compare outputs from MusicXML and shorthand inputs", "MakeScore")
{
  MakeScore xmlScore;
  REQUIRE(xmlScore.LoadXml("test1.musicxml"));

  MakeScore shorthandScore("4/4 clef-t key-s-0 <sb> 60");
  shorthandScore.MakeInternal();

  REQUIRE(xmlScore.ToString() == shorthandScore.ToString());
}

