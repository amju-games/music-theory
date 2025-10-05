// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman
//
// Unit tests for MakeScore

#include "catch.hpp"
#include "../MakeScore.h" 
#include "../Part.h" 

TEST_CASE("Test Part", "Part")
{
  MakeScore ms;
  REQUIRE(ms.NumParts() == 0);
  ms.AddPart("Piano"); // Parts have names
  REQUIRE(ms.NumParts() == 1);
  //ms.AddPart();
  //REQUIRE(ms.GetPart(0) != ms.GetPart(1));
}

