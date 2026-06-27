#include <iostream>
#include "catch.hpp"
#include "KeySig.h"

using namespace Catch::Matchers;

TEST_CASE("Parse key sig", "[KeySig]")
{
  REQUIRE(GetKeySig("key-s-0") == KeySig::KEYSIG_0_SHARP);
  REQUIRE(GetKeySig("key-s-1") == KeySig::KEYSIG_1_SHARP);
  REQUIRE(GetKeySig("key-s-2") == KeySig::KEYSIG_2_SHARP);
  REQUIRE(GetKeySig("key-s-3") == KeySig::KEYSIG_3_SHARP);
  REQUIRE(GetKeySig("key-s-4") == KeySig::KEYSIG_4_SHARP);
  REQUIRE(GetKeySig("key-s-5") == KeySig::KEYSIG_5_SHARP);
  REQUIRE(GetKeySig("key-s-6") == KeySig::KEYSIG_6_SHARP);
  REQUIRE(GetKeySig("key-s-7") == KeySig::KEYSIG_7_SHARP);

  REQUIRE(GetKeySig("key-f-0") == KeySig::KEYSIG_0_FLAT);
  REQUIRE(GetKeySig("key-f-1") == KeySig::KEYSIG_1_FLAT);
  REQUIRE(GetKeySig("key-f-2") == KeySig::KEYSIG_2_FLAT);
  REQUIRE(GetKeySig("key-f-3") == KeySig::KEYSIG_3_FLAT);
  REQUIRE(GetKeySig("key-f-4") == KeySig::KEYSIG_4_FLAT);
  REQUIRE(GetKeySig("key-f-5") == KeySig::KEYSIG_5_FLAT);
  REQUIRE(GetKeySig("key-f-6") == KeySig::KEYSIG_6_FLAT);
  REQUIRE(GetKeySig("key-f-7") == KeySig::KEYSIG_7_FLAT);
}

TEST_CASE("Output key sig, no clef", "[KeySig]")
{
  // No clef: can't output key sig for any key sig value.
  REQUIRE(GetKeySigOutputString(KeySig::KEYSIG_0_SHARP, Clef::CLEF_NONE, 0, 0, 1) == "");
  REQUIRE(GetKeySigOutputString(KeySig::KEYSIG_5_FLAT, Clef::CLEF_NONE, 0, 0, 1) == "");
}

TEST_CASE("Output key sig, treble clef, sharps", "[KeySig]")
{
  // No sharps or flats: output is empty string
  REQUIRE(GetKeySigOutputString(KeySig::KEYSIG_0_SHARP, Clef::CLEF_TREBLE, 0, 0, 1) == "");

  REQUIRE_THAT(GetKeySigOutputString(KeySig::KEYSIG_1_SHARP, Clef::CLEF_TREBLE), Contains("keysig-1sharp-treble"));
  REQUIRE_THAT(GetKeySigOutputString(KeySig::KEYSIG_2_SHARP, Clef::CLEF_TREBLE), Contains("keysig-2sharp-treble"));
  REQUIRE_THAT(GetKeySigOutputString(KeySig::KEYSIG_3_SHARP, Clef::CLEF_TREBLE), Contains("keysig-3sharp-treble"));
  REQUIRE_THAT(GetKeySigOutputString(KeySig::KEYSIG_4_SHARP, Clef::CLEF_TREBLE), Contains("keysig-4sharp-treble"));
  REQUIRE_THAT(GetKeySigOutputString(KeySig::KEYSIG_5_SHARP, Clef::CLEF_TREBLE), Contains("keysig-5sharp-treble"));
  REQUIRE_THAT(GetKeySigOutputString(KeySig::KEYSIG_6_SHARP, Clef::CLEF_TREBLE), Contains("keysig-6sharp-treble"));
  REQUIRE_THAT(GetKeySigOutputString(KeySig::KEYSIG_7_SHARP, Clef::CLEF_TREBLE), Contains("keysig-7sharp-treble"));
}

TEST_CASE("Transpose key sig", "[KeySig]")
{
  REQUIRE(TransposeKeySig(KeySig::KEYSIG_0_SHARP, 0) == KeySig::KEYSIG_0_SHARP);
  REQUIRE(TransposeKeySig(KeySig::KEYSIG_1_SHARP, 0) == KeySig::KEYSIG_1_SHARP);
  REQUIRE(TransposeKeySig(KeySig::KEYSIG_2_SHARP, 0) == KeySig::KEYSIG_2_SHARP);
  REQUIRE(TransposeKeySig(KeySig::KEYSIG_3_SHARP, 0) == KeySig::KEYSIG_3_SHARP);
  REQUIRE(TransposeKeySig(KeySig::KEYSIG_4_SHARP, 0) == KeySig::KEYSIG_4_SHARP);
  REQUIRE(TransposeKeySig(KeySig::KEYSIG_5_SHARP, 0) == KeySig::KEYSIG_5_SHARP);
  REQUIRE(TransposeKeySig(KeySig::KEYSIG_6_SHARP, 0) == KeySig::KEYSIG_6_SHARP);
  REQUIRE(TransposeKeySig(KeySig::KEYSIG_7_SHARP, 0) == KeySig::KEYSIG_7_SHARP);
}

