#include "catch.hpp"
#include "TimeSig.h"

using namespace MidiScore;

TEST_CASE("Beats in bar for time sig", "[TimeSig]")
{
  REQUIRE(BeatsInBar(TimeSig::TS_NONE) == 1); // ? zero could cause probs
  REQUIRE(BeatsInBar(TimeSig::TS_2_4) == 2);
  REQUIRE(BeatsInBar(TimeSig::TS_3_4) == 3);
  REQUIRE(BeatsInBar(TimeSig::TS_4_4) == 4);
  REQUIRE(BeatsInBar(TimeSig::TS_6_4) == 6);
  // These results are hopefully what makes sense for setting bar lines.
  REQUIRE(BeatsInBar(TimeSig::TS_3_8) == 1.5f);
  REQUIRE(BeatsInBar(TimeSig::TS_6_8) == 3.f);
  REQUIRE(BeatsInBar(TimeSig::TS_9_8) == 4.5f);
  REQUIRE(BeatsInBar(TimeSig::TS_12_8) == 6.f);
}

TEST_CASE("Time sig to string", "[TimeSig]")
{
  REQUIRE(TimeSigString(TimeSig::TS_NONE) == "");
  REQUIRE(TimeSigString(TimeSig::TS_2_4) == "2/4");
  REQUIRE(TimeSigString(TimeSig::TS_3_4) == "3/4");
  REQUIRE(TimeSigString(TimeSig::TS_4_4) == "4/4");
  REQUIRE(TimeSigString(TimeSig::TS_6_4) == "6/4");
  REQUIRE(TimeSigString(TimeSig::TS_3_8) == "3/8");
  REQUIRE(TimeSigString(TimeSig::TS_6_8) == "6/8");
  REQUIRE(TimeSigString(TimeSig::TS_9_8) == "9/8");
  REQUIRE(TimeSigString(TimeSig::TS_12_8) == "12/8");
}

TEST_CASE("Time sig from string", "[TimeSig]")
{
  REQUIRE(GetTimeSigFromString("1/4") == TimeSig::TS_NONE);
  REQUIRE(GetTimeSigFromString("2/4") == TimeSig::TS_2_4);
  REQUIRE(GetTimeSigFromString("3/4") == TimeSig::TS_3_4);
  REQUIRE(GetTimeSigFromString("4/4") == TimeSig::TS_4_4);
  REQUIRE(GetTimeSigFromString("6/4") == TimeSig::TS_6_4);
  REQUIRE(GetTimeSigFromString("3/8") == TimeSig::TS_3_8);
  REQUIRE(GetTimeSigFromString("6/8") == TimeSig::TS_6_8);
  REQUIRE(GetTimeSigFromString("9/8") == TimeSig::TS_9_8);
  REQUIRE(GetTimeSigFromString("12/8") == TimeSig::TS_12_8);
}

