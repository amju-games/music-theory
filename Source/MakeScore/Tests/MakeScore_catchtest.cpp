// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman
//
// Unit tests for MakeScore
// 
// To build on Mac:
//
//  clang++ *.cpp ../*cpp -DCATCH -std=c++11


#include "catch.hpp"
#include "../MakeScore.h" 

TEST_CASE("Test MakeScore", "MakeScore")
{
  MakeScore ms("c"); // one bar of one crotchet
  ms.MakeInternal();
  REQUIRE(ms.ToString() == "crotchet");


}
