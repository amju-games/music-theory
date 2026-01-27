// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include <iostream>
#include "catch.hpp"
#include "Event.h" 
#include "MidiScore.h"

using namespace MidiScore;

// Create one note event 
static Event n(int pitch, int start, int duration, int tpq)
{
  Event e;
  e.m_pitch = pitch;
  e.m_start = start;
  e.m_duration = duration;
  e.m_end = start + duration;
  e.SetTimeVal(tpq);
  return e;
}

TEST_CASE("Fill gaps with rests 1", "[Events]")
{ 
  const int tpq = 1; // ticks per quarter note
  Events events{ 
    n(60, 0, 4, tpq),
    n(62, 4, 2, tpq),
    n(64, 8, 4, tpq),
  };

  FillGapsWithRests(tpq, events);

  //std::cout << OutputEvents(events);

  REQUIRE(events.size() == 4);
  REQUIRE_FALSE(events[0].m_isRest);  // 60
  REQUIRE_FALSE(events[1].m_isRest);  // 62
  REQUIRE(events[2].m_isRest);  
  REQUIRE(events[2].m_start == 6);
  REQUIRE(events[2].m_duration == 2);
  REQUIRE_FALSE(events[3].m_isRest);  // 64
}

TEST_CASE("Fill gaps - long rests", "[Events]")
{ 
  const int tpq = 4; // ticks per quarter note
  Events events{ 
    n(60, 0,  4, tpq),  // pitch, start, duration, tpq
    n(62, 16, 4, tpq),  // 3 ticks gap
    n(64, 32, 4, tpq),
    n(65, 48, 4, tpq),
  };

  FillGapsWithRests(tpq, events);

  auto str = OutputEvents(events);

  //std::cout << str << "\n";  
}


