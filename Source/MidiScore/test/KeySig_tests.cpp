// * MidiScore *
// (c) Copyright 2026 Juliet Colman
//
// Unit tests for MidiScore
// 
// To build on Mac:
//
//  clang++ *.cpp ../*cpp -DCATCH -std=c++20
// 
// ..but there's a makefile and script to build and then run the tests.


#include "catch.hpp"
#include "KeySig.h" 

using namespace MidiScore;

// Create one note event - only pitch matters here
Event n(int pitch)
{
  Event e;
  e.m_pitch = pitch;
  return e;
}

Events MakeMajorScale(int p)
{
  return {n(p), n(p+2), n(p+4), n(p+5), n(p+7), n(p+9), n(p+11), n(p+12)};
}

TEST_CASE("Guess key sigs", "[KeySig]")
{
  REQUIRE(GuessKeySig(MakeMajorScale(60)) == KeySig::KS_SHARP_0);
  REQUIRE(GuessKeySig(MakeMajorScale(67)) == KeySig::KS_SHARP_1);
  REQUIRE(GuessKeySig(MakeMajorScale(62)) == KeySig::KS_SHARP_2);
  REQUIRE(GuessKeySig(MakeMajorScale(69)) == KeySig::KS_SHARP_3);
  REQUIRE(GuessKeySig(MakeMajorScale(64)) == KeySig::KS_SHARP_4);
  REQUIRE(GuessKeySig(MakeMajorScale(71)) == KeySig::KS_SHARP_5);
  REQUIRE(GuessKeySig(MakeMajorScale(66)) == KeySig::KS_SHARP_6);
  REQUIRE(GuessKeySig(MakeMajorScale(73)) == KeySig::KS_SHARP_7);

  REQUIRE(GuessKeySig(MakeMajorScale(65)) == KeySig::KS_FLAT_1);
  REQUIRE(GuessKeySig(MakeMajorScale(70)) == KeySig::KS_FLAT_2);
  REQUIRE(GuessKeySig(MakeMajorScale(63)) == KeySig::KS_FLAT_3);
  REQUIRE(GuessKeySig(MakeMajorScale(68)) == KeySig::KS_FLAT_4);
  // Specify that we prefer flat keys - otherwise we will match with enharmonic sharp keys.
  REQUIRE(GuessKeySig(MakeMajorScale(61), true) == KeySig::KS_FLAT_5);
  REQUIRE(GuessKeySig(MakeMajorScale(66), true) == KeySig::KS_FLAT_6);
  REQUIRE(GuessKeySig(MakeMajorScale(71), true) == KeySig::KS_FLAT_7);
}

TEST_CASE("Key sig strings", "[KeySig]")
{
  REQUIRE(KeySigString(KeySig::KS_SHARP_0) == "key-s-0");
  REQUIRE(KeySigString(KeySig::KS_SHARP_5) == "key-s-5");
  REQUIRE(KeySigString(KeySig::KS_FLAT_0) == "key-f-0");
  REQUIRE(KeySigString(KeySig::KS_FLAT_7) == "key-f-7");
}

