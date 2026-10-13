#include <iostream>
#include "catch.hpp"
#include "MidiScore.h"
#include "Staccato.h"
#include "Utils.h"

TEST_CASE("No staccato notes", "[Staccato]")
{
  using namespace MidiScore;

  const int tpq = 2; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0, 4, tpq), // m 
    n(62, 4, 4, tpq), // m
    n(64, 8, 8, tpq), // sb
  };

  // No gaps in notes, so no staccato notes found.
  int numStacc = StaccatoPass(events);
  REQUIRE(numStacc == 0);
}

TEST_CASE("Find staccato notes", "[Staccato]")
{
  using namespace MidiScore;

  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0, 2, tpq), // q 
    // Gap, same len as note 60
    n(62, 4, 4, tpq), // c
    // Gap, same len as note 62
    n(64, 12, 8, tpq), // m
    // Last note can't be flagged as stacc as no gap after.
  };

  REQUIRE(events[0].GetTimeVal() == TimeVal::QUAVER);
  REQUIRE(events[1].GetTimeVal() == TimeVal::CROTCHET);

  int numStacc = StaccatoPass(events);

  std::cout << OutputEvents(events) << "\n";

  REQUIRE(numStacc == 2);
  REQUIRE(events[0].m_isStacc);
  // Time val of note doubled, so there won't be a rest in between.
  REQUIRE(events[0].GetTimeVal() == TimeVal::CROTCHET);

  REQUIRE(events[1].m_isStacc);
  // Time val of note doubled, so there won't be a rest in between.
  REQUIRE(events[1].GetTimeVal() == TimeVal::MINIM);

  REQUIRE(!events[2].m_isStacc);
}

