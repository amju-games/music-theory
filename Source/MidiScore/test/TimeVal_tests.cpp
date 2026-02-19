#include "catch.hpp"
#include "Event.h"

using namespace MidiScore;

TEST_CASE("Time val to string", "[TimeVal]")
{
  // Convert internal duration value to string recognised by
  //  MakeScore. (Some of these strings actually are NOT -- they 
  //  are for future MakeScore expansion.)

  REQUIRE(TimeValString(TimeVal::QQQ) == "qqq");
  REQUIRE(TimeValString(TimeVal::SEMIQUAVER) == "qq");
  REQUIRE(TimeValString(TimeVal::QUAVER) == "q");
  REQUIRE(TimeValString(TimeVal::CROTCHET) == "c");
  REQUIRE(TimeValString(TimeVal::MINIM) == "m");
  REQUIRE(TimeValString(TimeVal::SEMIBREVE) == "sb");
  REQUIRE(TimeValString(TimeVal::SB2) == "sb2");
  REQUIRE(TimeValString(TimeVal::SB4) == "sb4");
}

