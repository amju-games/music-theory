#include <iostream>
#include "catch.hpp"
#include "Event.h"
#include "MidiScore.h"

using namespace MidiScore;

TEST_CASE("Split insert", "[Rests]")
{
  // Insert some rests into an empty event vec.
  Events events;

  int tpq = 16;
  const int start = 0;
  const bool notWholeBar = false;
  const bool allowDotted = false; 

  SECTION("Insert simple crotchet")
  {
    const int duration = tpq;

//    std::cout << "Inserting rest(s) to duration " << duration << "\n";

    auto it = SplitInsertRest(events, events.begin(),
      tpq, duration, start, notWholeBar, allowDotted);

//    std::cout << OutputEvents(events) << "\n";

    REQUIRE(events.size() == 1);
    REQUIRE(events[0].IsRest());
    REQUIRE(events[0].m_timeVal == TimeVal::CROTCHET);
  }

  SECTION("Insert crotchet + quaver + qq")
  {
    const int duration = tpq + tpq/2 + tpq/4;

//    std::cout << "Inserting rest(s) to duration " << duration << "\n";

    auto it = SplitInsertRest(events, events.begin(),
      tpq, duration, start, notWholeBar, allowDotted);

//    std::cout << OutputEvents(events) << "\n";

    REQUIRE(events.size() == 3);
    REQUIRE(events[0].IsRest());
    REQUIRE(events[0].m_timeVal == TimeVal::CROTCHET);
    REQUIRE(events[1].IsRest());
    REQUIRE(events[1].m_timeVal == TimeVal::QUAVER);
    REQUIRE(events[2].IsRest());
    REQUIRE(events[2].m_timeVal == TimeVal::SEMIQUAVER);
  }

  SECTION("Insert crotchet + quaver, allow dotted")
  {
    const int duration = tpq + tpq/2;

//    std::cout << "Inserting rest(s) to duration " << duration << "\n";

    const bool allowDotted = true; 

    auto it = SplitInsertRest(events, events.begin(),
      tpq, duration, start, notWholeBar, allowDotted);

//    std::cout << OutputEvents(events) << "\n";

    REQUIRE(events.size() == 1);
    REQUIRE(events[0].IsRest());
    REQUIRE(events[0].m_timeVal == TimeVal::CROTCHET);
    REQUIRE(events[0].m_dots == 1);
  }
}

