#include "catch.hpp"
#include "Event.h"

using namespace MidiScore;

TEST_CASE("Time val to string", "[TimeVal]")
{
  // Convert internal duration value to string recognised by
  //  MakeScore. (Some of these strings actually are NOT -- they 
  //  are for future MakeScore expansion.)

  // Dot defaults to 0
  REQUIRE(TimeValString(TimeVal::QQQ) == "qqq");
  REQUIRE(TimeValString(TimeVal::SEMIQUAVER) == "qq");
  REQUIRE(TimeValString(TimeVal::QUAVER) == "q");
  REQUIRE(TimeValString(TimeVal::CROTCHET) == "c");
  REQUIRE(TimeValString(TimeVal::MINIM) == "m");
  REQUIRE(TimeValString(TimeVal::SEMIBREVE) == "sb");
  REQUIRE(TimeValString(TimeVal::SB2) == "sb2");
  REQUIRE(TimeValString(TimeVal::SB4) == "sb4");

  REQUIRE(TimeValString(TimeVal::QQQ, 1) == "qqq.");
  REQUIRE(TimeValString(TimeVal::SEMIQUAVER, 1) == "qq.");
  REQUIRE(TimeValString(TimeVal::QUAVER, 1) == "q.");
  REQUIRE(TimeValString(TimeVal::CROTCHET, 1) == "c.");
  REQUIRE(TimeValString(TimeVal::MINIM, 1) == "m.");
  REQUIRE(TimeValString(TimeVal::SEMIBREVE, 1) == "sb.");
  REQUIRE(TimeValString(TimeVal::SB2, 1) == "sb2.");
  REQUIRE(TimeValString(TimeVal::SB4, 1) == "sb4.");

  // Multiple dots are supported, but not used
  REQUIRE(TimeValString(TimeVal::CROTCHET, 2) == "c..");
}

