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

TEST_CASE("Insert one rest", "[Events]")
{ 
  const int tpq = 1; // ticks per quarter note
  Events events
  { 
    n(60, 0, 4, tpq), // sb
    n(62, 4, 2, tpq), // m
    n(64, 8, 4, tpq), // sb
  };

  InsertRests(tpq, events);

  //std::cout << OutputEvents(events);

  REQUIRE(events.size() == 4);
  REQUIRE(events[0].IsNote());  // 60
  REQUIRE(events[1].IsNote());  // 62
  REQUIRE(events[2].IsRest());  
  REQUIRE(events[2].m_start == 6);
  REQUIRE(events[2].m_duration == 2);
  REQUIRE(events[3].IsNote());  // 64
}

TEST_CASE("Insert rests with bar lines", "[Events]")
{ 
  const int tpq = 4; // ticks per quarter note
  Events events{ 
    // pitch, start, duration, tpq
    n(60, 4,  4, tpq),  // cr, c, mr
    n(62, 20, 8, tpq),  // cr, m, cr
    n(64, 44, 4, tpq),  // mr. c
    n(65, 48, 4, tpq),  // c mr
  };

  // Add bar lines as this is the 'real world' situation
  InsertBarLines(tpq, TimeSig::TS_4_4, events);
  InsertRests(tpq, events);

  //std::cout << OutputEvents(events);
  // cr <c> 60 mr | cr <m> 62 cr | mr. <c> 64 | <c> 65 mr. | 

  REQUIRE(events.size() == 14);
  REQUIRE(events[0].IsRest());
  REQUIRE(events[1].IsNote());
  REQUIRE(events[2].IsRest());
  REQUIRE(events[3].IsBarLine());
  REQUIRE(events[4].IsRest());
  REQUIRE(events[5].IsNote());
  REQUIRE(events[6].IsRest());
  REQUIRE(events[7].IsBarLine());
  REQUIRE(events[8].IsRest());
  REQUIRE(events[9].IsNote());
  REQUIRE(events[10].IsBarLine());
  REQUIRE(events[11].IsNote());
  REQUIRE(events[12].IsRest());
  REQUIRE(events[13].IsBarLine());
}

TEST_CASE("Add bar lines 4/4", "[Events]")
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

  // Not inserting rests, and notes fall nicely within bar lines, so
  //  output is simply the notes interleaved with bar lines.
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

TEST_CASE("Add bar lines 3/4", "[Events]")
{
  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0,  16, tpq),  // sb
    n(62, 16, 12, tpq),  // m.  cr
    n(64, 32, 8, tpq),   // m   mr
    n(65, 48, 4, tpq),   // c   (final rest length is whatever fills up the last bar). 
  };

  InsertBarLines(tpq, TimeSig::TS_3_4, events);
  // Same note events as in the 4/4 test, but now 
  //  the notes don't fall nicely within bars - add rests to show
  //  the timing.
  InsertRests(tpq, events);

  //std::cout << OutputEvents(events);
  // <m.> 60 | t <c> 60 <m> 62 | t <c> 62 cr <c> 64 | t <c> 64 mr | <c> 65 mr |

  REQUIRE(events.size() == 18);
  REQUIRE(events[0].IsNote());
  REQUIRE(events[1].IsBarLine());
  REQUIRE(events[2].IsTie());
  REQUIRE(events[3].IsNote());
  REQUIRE(events[4].IsNote());
  REQUIRE(events[5].IsBarLine());
  REQUIRE(events[6].IsTie());
  REQUIRE(events[7].IsNote());
  REQUIRE(events[8].IsRest());
  REQUIRE(events[9].IsNote());
  REQUIRE(events[10].IsBarLine());
  REQUIRE(events[11].IsTie());
  REQUIRE(events[12].IsNote());
  REQUIRE(events[13].IsRest());
  REQUIRE(events[14].IsBarLine());
  REQUIRE(events[15].IsNote());
  REQUIRE(events[16].IsRest());
  REQUIRE(events[17].IsBarLine());
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
  REQUIRE(events[1].IsNote());
  REQUIRE(events[2].IsNote());
  REQUIRE(events[3].IsChordEnd());
  REQUIRE(events[4].IsNote());
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
    n(60, 0,  8, tpq),  
    n(64, 0,  16, tpq),  
  };

  InsertChordMarkers(events);
  //std::cout << OutputEvents(events);

  REQUIRE(events.size() == 4);
  REQUIRE(events[0].IsChordStart());
  // Notes in chord are sorted so longest duration comes first 
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

TEST_CASE("Chord split across bar line", "[Events]")
{
  // Expect e.g. <sb> ( 60 64 ) -> <m> ( 60 64 ) | t <m> ( 60 64 )
  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0,  32, tpq),  // double sb
    n(64, 0,  32, tpq),  
  };

  InsertChordMarkers(events);
  InsertBarLines(tpq, TimeSig::TS_4_4, events);

  //std::cout << "After bar line/splitting: " << OutputEvents(events) << "\n";

  REQUIRE(events.size() == 11);
  REQUIRE(events[0].IsChordStart());
  REQUIRE(events[1].IsNote());
  REQUIRE(events[2].IsNote());
  REQUIRE(events[3].IsChordEnd());
  REQUIRE(events[4].IsBarLine());
  REQUIRE(events[5].IsTie());
  REQUIRE(events[6].IsChordStart());
  REQUIRE(events[7].IsNote());
  REQUIRE(events[8].IsNote());
  REQUIRE(events[9].IsChordEnd());
  REQUIRE(events[10].IsBarLine());
}

TEST_CASE("Chord split across bar line, different note durations", "[Events]")
{
  // Expect e.g. <sb> ( 60 64 ) -> <m> ( 60 64 ) | t <m> ( 60 64 )
  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0,  32, tpq),  // double sb
    n(64, 0,  16, tpq),  // single sb
    n(67, 0,  8, tpq),  // minim 
  };

  InsertChordMarkers(events);
  InsertBarLines(tpq, TimeSig::TS_4_4, events);

  //std::cout << "After bar line/splitting: " << OutputEvents(events) << "\n";

  REQUIRE(events.size() == 9);
  REQUIRE(events[0].IsChordStart());
  REQUIRE(events[1].IsNote());
  REQUIRE(events[2].IsNote());
  REQUIRE(events[3].IsNote());
  REQUIRE(events[4].IsChordEnd());
  REQUIRE(events[5].IsBarLine());
  REQUIRE(events[6].IsTie());
  REQUIRE(events[7].IsNote()); // only one of the notes 'survives' into the next bar
  REQUIRE(events[8].IsBarLine());
}

TEST_CASE("Chord NOT split across bar lines", "[Events]")
{
  // Expect e.g. ( <sb> 60 64 ) | ( <sb> 61 65 ) | 
  // i.e. bar line is not within chord markers.

  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0,  16, tpq),  // sb
    n(64, 0,  16, tpq),  
    n(61, 16,  16, tpq),  // sb, next bar
    n(65, 16,  16, tpq),  
  };

  InsertChordMarkers(events);
  InsertBarLines(tpq, TimeSig::TS_4_4, events);

  //std::cout << "After bar line/splitting: " << OutputEvents(events) << "\n";

  REQUIRE(events.size() == 10);
  REQUIRE(events[0].IsChordStart());
  REQUIRE(events[1].IsNote());
  REQUIRE(events[2].IsNote());
  REQUIRE(events[3].IsChordEnd());
  REQUIRE(events[4].IsBarLine());
  REQUIRE(events[5].IsChordStart());
  REQUIRE(events[6].IsNote());
  REQUIRE(events[7].IsNote());
  REQUIRE(events[8].IsChordEnd());
  REQUIRE(events[9].IsBarLine());
}

