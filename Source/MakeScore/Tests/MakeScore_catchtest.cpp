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


