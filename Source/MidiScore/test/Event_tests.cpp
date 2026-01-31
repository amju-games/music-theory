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
  e.m_type = EventType::NOTE;
  e.m_pitch = pitch;
  e.m_start = start;
  e.m_duration = duration;
  e.m_end = start + duration;
  e.SetTimeVal(tpq);
  return e;
}

// Create a rest event
static Event r(int start, int duration, int tpq)
{
  Event e;
  e.m_type = EventType::REST;
  e.m_start = start;
  e.m_duration = duration;
  e.m_end = start + duration;
  e.SetTimeVal(tpq);
  return e;
}

// Test basic stuff, the output string for notes etc.
TEST_CASE("Output strings", "[Events]")
{
  // n(pitch, start, duration, tpq) 

  // TPQ needs to be >1, because we do some integer arithmetic to work 
  //  out if we should add a dot. If TPQ is 1, 1.5*1 == 1 in integer
  //  land, and we erroneously add a dot.  
  REQUIRE(n(60, 0, 4, 4).ToString() == "<c> 60"); // crotchet, pitch 60

  REQUIRE(n(60, 0, 8, 4).ToString() == "<m> 60"); // minim
  REQUIRE(n(60, 0, 16, 4).ToString() == "<sb> 60"); // semibreve
  REQUIRE(n(60, 0, 32, 4).ToString() == "<sb2> 60"); // double semibreve
  REQUIRE(n(60, 0, 64, 4).ToString() == "<sb4> 60"); // quadruple semibreve
  REQUIRE(n(60, 0, 2, 4).ToString() == "<q> 60"); // quaver
  // We need a TPQ of at least 8 for semiquavers to work, again because of integer
  //  arithmetic.
  REQUIRE(n(60, 0, 2, 8).ToString() == "<qq> 60"); // semiquaver
  // TODO What about durations above sb4 and below qq? 

  // Dotted durations
  REQUIRE(n(60, 0, 6, 4).ToString() == "<c.> 60"); // dotted crotchet
  REQUIRE(n(60, 0, 12, 4).ToString() == "<m.> 60"); // dotted minim
  REQUIRE(n(60, 0, 24, 4).ToString() == "<sb.> 60"); // dotted semibreve
  REQUIRE(n(60, 0, 48, 4).ToString() == "<sb2.> 60"); // dotted double semibreve
  REQUIRE(n(60, 0, 96, 4).ToString() == "<sb4.> 60"); // dotted quadruple semibreve
  REQUIRE(n(60, 0, 3, 4).ToString() == "<q.> 60"); // dotted quaver
  // For qqs we need a TPQ of at least 8
  REQUIRE(n(60, 0, 3, 8).ToString() == "<qq.> 60"); // dotted semiquaver

  // Rests
  REQUIRE(r(0, 4, 4).ToString() == "cr"); // crotchet rest
  REQUIRE(r(0, 8, 4).ToString() == "mr"); // minim rest
  REQUIRE(r(0, 16, 4).ToString() == "sbr"); // semibreve rest
  REQUIRE(r(0, 32, 4).ToString() == "sb2r"); // sb2 rest
  REQUIRE(r(0, 64, 4).ToString() == "sb4r"); // sb4 rest
  REQUIRE(r(0, 2, 4).ToString() == "qr"); // quaver rest
  REQUIRE(r(0, 2, 8).ToString() == "qqr"); // semiquaver rest

  REQUIRE(r(0, 6, 4).ToString() == "c.r"); // dotted crotchet rest
  REQUIRE(r(0, 12, 4).ToString() == "m.r"); // dotted minim rest
  REQUIRE(r(0, 24, 4).ToString() == "sb.r"); // dotted semibreve rest
  REQUIRE(r(0, 48, 4).ToString() == "sb2.r"); // dotted sb2 rest
  REQUIRE(r(0, 96, 4).ToString() == "sb4.r"); // dotted sb4 rest
  REQUIRE(r(0, 3, 4).ToString() == "q.r"); // dotted quaver rest
  REQUIRE(r(0, 3, 8).ToString() == "qq.r"); // dotted semiquaver rest
}

// Showing what happens, this is not good behaviour, but shows the edge cases.
TEST_CASE("BAD Output strings", "[Events]")
{
  // TPQ is 1, int arithmetic fails and we erroneously add a dot.
  REQUIRE(n(60, 0, 1, 1).ToString() == "<c.> 60"); // s/b "<c> 60"
  // TPQ of 2 gives correct string for a crotchet
  REQUIRE(n(60, 0, 2, 2).ToString() == "<c> 60"); // s/b "<c> 60"

  // TPQ needs to be >=4 for a semiquaver, (so we can express 1/4 of a crotchet)
  //  -- but this erroneously adds a dot, so TPQ needs to be >=8. 
  REQUIRE(n(60, 0, 1, 4).ToString() == "<qq.> 60"); // s/b "<qq> 60"

  // Limit of small note values: demisemiquaver is not recognised.
  // Duration is 1/8 of a crotchet, should be "qqq"?
  REQUIRE(n(60, 0, 1, 8).ToString() == "<qq> 60"); // s/b "<qqq> 60" ?

  // Limit of large note values
  // Above a dotted sb4, note values are not recognised.
  // Duration is 32 crotchets (128/4), 8 semibreves.
  REQUIRE(n(60, 0, 128, 4).ToString() == "<sb4> 60"); // s/b "<sb8> 60" ?
}

TEST_CASE("Insert one rest", "[Events]")
{ 
  const int tpq = 1; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0, 4, tpq), // sb
    n(62, 4, 2, tpq), // m
    // gap here! minim ends at t=6, next note starts at t=8
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
    // There are some gaps between note (n) end time and note (n+1) start time.
    // But we are not testing inserting rests here, just bar lines.

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
  //  the timing, or it's hard to make sense of the result.
  InsertRests(tpq, events);

  //std::cout << OutputEvents(events);
  // Each bar is 3 crotchets duration; notes are split and tied across bar lines.
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
  // Simplest test showing a note split across a bar line

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
  // Super long note split across multiple bars

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
  // Identify a chord - notes have the same start times.

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
  // Test edge case: first and final notes in piece are a chord

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
  // Notes in a chord can have different durations. 
  // To make subsequent processing easier, the notes are sorted so
  //  the note with the max duration comes first.
  
  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0,  8, tpq),  // shorter note
    n(64, 0,  16, tpq),  // longer note
  };

  InsertChordMarkers(events);
  //std::cout << OutputEvents(events);

  REQUIRE(events.size() == 4);
  REQUIRE(events[0].IsChordStart());
  // Notes in chord are sorted so longest duration comes first 
  REQUIRE(events[1].m_duration == 16); // longer
  REQUIRE(events[2].m_duration == 8); // shorter
  REQUIRE(events[3].IsChordEnd());
  // Chord end marker has end time of the longest note
  REQUIRE(events[3].m_end == 16); 
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

// Time Set events are added when the start time of an event isn't simply the 
//  start time + duration of the previous event.
// Time Set value is number of crotchets since start of piece, zero based. 
//  Can have a fractional part. 
TEST_CASE("Time Set: Chord, different durations, with extra note in chord", "[Events]")
{
  // Chord with another note starting within the chord markers.
    
  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0,  16, tpq),  // sb - two note chord with sb and m notes
    n(64, 0,  8, tpq),   // m
    n(62, 12,  4, tpq),  // c, 4th beat of same bar, so within chord markers
  };

  InsertChordMarkers(events);
  InsertBarLines(tpq, TimeSig::TS_4_4, events);
  InsertRests(tpq, events);
  InsertTimeSetEvents(tpq, events); // need time set event to correctly place final note

  auto str = OutputEvents(events);
  //std::cout << str;
  // Expect time set event to place final note; time set value is in crotchets
  const std::string expected = "( <sb> 60 <m> 64 ) time 3 <c> 62";
  REQUIRE(str.substr(0, expected.size()) == expected);
}

TEST_CASE("Time Set: Chord, different durations, with note after shorter note in chord", "[Events]")
{
  // Chord with another note starting within the chord markers.
    
  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0,  16, tpq),  // sb - two note chord with sb and m notes
    n(64, 0,  8, tpq),   // m
    n(62, 8,  4, tpq),  // c, 3rd beat of same bar, so within chord markers
  };

  InsertChordMarkers(events);
  InsertBarLines(tpq, TimeSig::TS_4_4, events);
  InsertRests(tpq, events);
  InsertTimeSetEvents(tpq, events);

  auto str = OutputEvents(events);
  //std::cout << str;
  // Expect time set event to correctly place the 3rd note.
  const std::string expected = "( <sb> 60 <m> 64 ) time 2 <c> 62"; 
  REQUIRE(str.substr(0, expected.size()) == expected);
}

TEST_CASE("Time Set event, not within chord", "[Events]")
{
  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0,  16, tpq),  // sb 
    n(62, 8,  4, tpq),  // c, 3rd beat of same bar
  };

  InsertChordMarkers(events);
  InsertBarLines(tpq, TimeSig::TS_4_4, events);
  InsertRests(tpq, events);
  InsertTimeSetEvents(tpq, events);

  auto str = OutputEvents(events);
  //std::cout << str;
  // Time set value is in crotchets from start of piece
  const std::string expected = "<sb> 60 time 2 <c> 62";
  REQUIRE(str.substr(0, expected.size()) == expected);
}

TEST_CASE("Time Set event, fractional value", "[Events]")
{
  const int tpq = 4; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0,  16, tpq),  // sb 
    n(62, 2,  2, tpq),  // quaver, 1/2 a crotchet from start
  };

  InsertChordMarkers(events);
  InsertBarLines(tpq, TimeSig::TS_4_4, events);
  InsertRests(tpq, events);
  InsertTimeSetEvents(tpq, events);

  auto str = OutputEvents(events);
  //std::cout << str;
  // Time set value is in crotchets, so we expect time set of 0.5 
  const std::string expected = "<sb> 60 time 0.5 <q> 62";
  REQUIRE(str.substr(0, expected.size()) == expected);
}

