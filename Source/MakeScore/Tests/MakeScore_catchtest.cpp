// * Amjula music theory *
// (c) Copyright 2017 Jason Colman
//
// Unit tests for MakeScore
// 
// To build on Mac:
//
//  clang++ *.cpp -DCATCH -std=c++11


#include "catch.hpp"
#include "../MakeScore.cpp" // ! There is no .h file at present!

TEST_CASE("Test MakeScore", "MakeScore")
{
  MakeScore ms("c"); // one bar of one crotchet
  ms.MakeInternal();
  REQUIRE(ms.ToString() == "crotchet");


}
