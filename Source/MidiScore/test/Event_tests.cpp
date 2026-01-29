// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include <iostream>
#include "catch.hpp"
#include "Event.h" 
#include "MidiScore.h"
#include "TimeSig.h"

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
  Events events
  { 
    n(60, 0, 4, tpq),
    n(62, 4, 2, tpq),
    n(64, 8, 4, tpq),
  };

  InsertRests(tpq, events);

  //std::cout << OutputEvents(events);

  REQUIRE(events.size() == 4);
  REQUIRE_FALSE(events[0].IsRest());  // 60
  REQUIRE_FALSE(events[1].IsRest());  // 62
  REQUIRE(events[2].IsRest());  
  REQUIRE(events[2].m_start == 6);
  REQUIRE(events[2].m_duration == 2);
  REQUIRE_FALSE(events[3].IsRest());  // 64
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

  InsertRests(tpq, events);

  REQUIRE(events.size() == 7); // rest not added at end, so 3 rests added
  REQUIRE(events[0].IsNote());
  REQUIRE(events[1].IsRest());
  REQUIRE(events[2].IsNote());
  REQUIRE(events[3].IsRest());
  REQUIRE(events[4].IsNote());
  REQUIRE(events[5].IsRest());
  REQUIRE(events[6].IsNote());
}

TEST_CASE("Add bar lines", "[Events]")
{
  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0,  16, tpq),  // sb
    n(62, 16, 12, tpq),  // m.
    n(64, 32, 8, tpq),   // m
    n(65, 48, 4, tpq),   // c
  };

  InsertBarLines(tpq, TimeSig::TS_4_4, events);

  //std::cout << OutputEvents(events);

  REQUIRE(events.size() == 8);
  REQUIRE(events[0].IsNote());
  REQUIRE(events[1].IsBarLine());
  REQUIRE(events[2].IsNote());
  REQUIRE(events[3].IsBarLine());
  REQUIRE(events[4].IsNote());
  REQUIRE(events[5].IsBarLine());
  REQUIRE(events[6].IsNote());
  REQUIRE(events[7].IsBarLine());
}

TEST_CASE("Adding bar lines splits notes", "[Events]")
{
  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 8,  16, tpq),  // sb starting on beat 3 
  };

  InsertBarLines(tpq, TimeSig::TS_4_4, events);
  //InsertRests(tpq, events); 
  //std::cout << OutputEvents(events);

  REQUIRE(events.size() == 5);
  REQUIRE(events[0].IsNote());
  REQUIRE(events[1].IsBarLine());
  REQUIRE(events[2].IsTie());
  REQUIRE(events[3].IsNote());
  REQUIRE(events[4].IsBarLine());
}

TEST_CASE("Split note across multiple bars", "[Events]")
{
  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 8,  32, tpq),  // double-sb starting on beat 3 
  };

  InsertBarLines(tpq, TimeSig::TS_4_4, events);
  //InsertRests(tpq, events); 
  //std::cout << OutputEvents(events);

  REQUIRE(events.size() == 8);
  REQUIRE(events[0].IsNote());
  REQUIRE(events[1].IsBarLine());
  REQUIRE(events[2].IsTie());
  REQUIRE(events[3].IsNote());
  REQUIRE(events[4].IsBarLine());
  REQUIRE(events[5].IsTie());
  REQUIRE(events[6].IsNote());
  REQUIRE(events[7].IsBarLine());
}

TEST_CASE("Chord - 2 notes", "[Events]")
{
  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0,  16, tpq),  
    n(64, 0,  16, tpq),  
    n(67, 16,  16, tpq),  
  };

  InsertChordMarkers(events);
  //std::cout << OutputEvents(events);

  REQUIRE(events.size() == 5);
  REQUIRE(events[0].IsChordStart());
  REQUIRE(events[3].IsChordEnd());
}

TEST_CASE("Chord at end of piece", "[Events]")
{
  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0,  16, tpq),  
    n(64, 0,  16, tpq),  
  };

  InsertChordMarkers(events);
  //std::cout << OutputEvents(events);

  REQUIRE(events.size() == 4);
  REQUIRE(events[0].IsChordStart());
  REQUIRE(events[3].IsChordEnd());
}

TEST_CASE("Chord: different note durations", "[Events]")
{
  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0,  16, tpq),  
    n(64, 0,  8, tpq),  
  };

  InsertChordMarkers(events);
  //std::cout << OutputEvents(events);

  REQUIRE(events.size() == 4);
  REQUIRE(events[0].IsChordStart());
  REQUIRE(events[1].m_duration == 16);
  REQUIRE(events[2].m_duration == 8); 
  REQUIRE(events[3].IsChordEnd());
}

TEST_CASE("Big Chord", "[Events]")
{
  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0,  4, tpq),  
    n(60, 4,  4, tpq),  
    n(64, 4,  4, tpq),  
    n(67, 4,  4, tpq),  
    n(72, 4,  4, tpq),  
    n(60, 8,  4, tpq),
  };

  InsertChordMarkers(events);
  //std::cout << OutputEvents(events);

  REQUIRE(events.size() == 8);
  REQUIRE(events[0].IsNote());
  REQUIRE(events[1].IsChordStart());
  REQUIRE(events[6].IsChordEnd());
  REQUIRE(events[7].IsNote());
}

