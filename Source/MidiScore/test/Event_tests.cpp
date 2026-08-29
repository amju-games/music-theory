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
  e.m_unquantisedStart = start;
  e.m_start = start; 
  e.m_unquantisedDuration = duration;
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

// Create a barline event
static Event barline(int start, int tpq)
{
  Event e;
  e.m_type = EventType::BARLINE;
  e.m_start = start;
  e.m_duration = 0; 
  e.m_end = start + e.m_duration;
  e.SetTimeVal(tpq);
  return e;
}

TEST_CASE("CalcEndOfBar helper function", "[Events]")
{
  const int tpq = 480;
  REQUIRE(CalcEndOfBar(tpq, 0   * tpq, TimeSig::TS_4_4) == 4 * tpq);
  REQUIRE(CalcEndOfBar(tpq, 7   * tpq, TimeSig::TS_4_4) == 8 * tpq);
  REQUIRE(CalcEndOfBar(tpq, 8   * tpq, TimeSig::TS_4_4) == 12 * tpq);

  REQUIRE(CalcEndOfBar(tpq, 4   * tpq, TimeSig::TS_3_4) == 6 * tpq);

  REQUIRE(CalcEndOfBar(tpq, 5   * tpq, TimeSig::TS_2_4) == 6 * tpq);
}

TEST_CASE("CalcEndOfBar with anacrusis", "[Events]")
{
  const int tpq = 480;
  // Anacrusis length 1 crotchet, pos is zero: result is simply anac. length
  REQUIRE(CalcEndOfBar(tpq, 0   * tpq, TimeSig::TS_4_4, 1 * tpq) == 1 * tpq);

  // Anac. 1 minim, pos 1 crotchet: result is anac. length again
  REQUIRE(CalcEndOfBar(tpq, 1   * tpq, TimeSig::TS_4_4, 2 * tpq) == 2 * tpq);

  // Anac. 1 minim, pos 7 crotchets, in 4/4
  // 0  1  2  3  4  5  6  7  8  9  10
  //       |           |  *        |
  REQUIRE(CalcEndOfBar(tpq, 7   * tpq, TimeSig::TS_4_4, 2 * tpq) == 10 * tpq);
}

// Test basic stuff, the output string for notes etc.
TEST_CASE("Output strings", "[Events]")
{
  // n(pitch, start, duration, tpq) 

  // TPQ needs to be >1, because we do some integer arithmetic to work 
  //  out if we should add a dot. If TPQ is 1, 1.5*1 == 1 in integer
  //  land, and we erroneously add a dot.  
  REQUIRE(n(60, 0, 4, 4).ToString() == "<c> c4"); // crotchet, pitch 60

  REQUIRE(n(60, 0, 8, 4).ToString() == "<m> c4"); // minim
  REQUIRE(n(60, 0, 16, 4).ToString() == "<sb> c4"); // semibreve
  REQUIRE(n(60, 0, 32, 4).ToString() == "<sb2> c4"); // double semibreve
  REQUIRE(n(60, 0, 64, 4).ToString() == "<sb4> c4"); // quadruple semibreve
  REQUIRE(n(60, 0, 2, 4).ToString() == "<q> c4"); // quaver
  // We need a TPQ of at least 8 for semiquavers to work, again because of integer
  //  arithmetic.
  REQUIRE(n(60, 0, 2, 8).ToString() == "<qq> c4"); // semiquaver
  // TODO What about durations above sb4 and below qq? 

  // Dotted durations
  REQUIRE(n(60, 0, 6, 4).ToString() == "<c.> c4"); // dotted crotchet
  REQUIRE(n(60, 0, 12, 4).ToString() == "<m.> c4"); // dotted minim
  REQUIRE(n(60, 0, 24, 4).ToString() == "<sb.> c4"); // dotted semibreve
  REQUIRE(n(60, 0, 48, 4).ToString() == "<sb2.> c4"); // dotted double semibreve
  REQUIRE(n(60, 0, 96, 4).ToString() == "<sb4.> c4"); // dotted quadruple semibreve
  REQUIRE(n(60, 0, 3, 4).ToString() == "<q.> c4"); // dotted quaver
  // For qqs we need a TPQ of at least 8
  REQUIRE(n(60, 0, 3, 8).ToString() == "<qq.> c4"); // dotted semiquaver

  // Rests
  REQUIRE(r(0, 4, 4).ToString() == "<c> r"); // crotchet rest
  REQUIRE(r(0, 8, 4).ToString() == "<m> r"); // minim rest
  REQUIRE(r(0, 16, 4).ToString() == "<sb> r"); // semibreve rest
  REQUIRE(r(0, 32, 4).ToString() == "<sb2> r"); // sb2 rest
  REQUIRE(r(0, 64, 4).ToString() == "<sb4> r"); // sb4 rest
  REQUIRE(r(0, 2, 4).ToString() == "<q> r"); // quaver rest
  REQUIRE(r(0, 2, 8).ToString() == "<qq> r"); // semiquaver rest

  REQUIRE(r(0, 6, 4).ToString() == "<c.> r"); // dotted crotchet rest
  REQUIRE(r(0, 12, 4).ToString() == "<m.> r"); // dotted minim rest
  REQUIRE(r(0, 24, 4).ToString() == "<sb.> r"); // dotted semibreve rest
  REQUIRE(r(0, 48, 4).ToString() == "<sb2.> r"); // dotted sb2 rest
  REQUIRE(r(0, 96, 4).ToString() == "<sb4.> r"); // dotted sb4 rest
  REQUIRE(r(0, 3, 4).ToString() == "<q.> r"); // dotted quaver rest
  REQUIRE(r(0, 3, 8).ToString() == "<qq.> r"); // dotted semiquaver rest
}

// Showing what happens, this is not good behaviour, but shows the edge cases.
TEST_CASE("BAD Output strings 1", "[Events]")
{
  // n(pitch, start, duration, tpq) 

  // TPQ is 1, int arithmetic fails and we erroneously add a dot?
  // Actually that seeems to be ok now.
  REQUIRE(n(60, 0, 1, 1).ToString() == "<c> c4"); // s/b "<c> 60"
  // TPQ of 2 gives correct string for a crotchet
  REQUIRE(n(60, 0, 2, 2).ToString() == "<c> c4"); // s/b "<c> 60"
}

TEST_CASE("BAD Output strings 2", "[Events]")
{
  // TPQ needs to be >=4 for a semiquaver, (so we can express 1/4 of a crotchet)
  //  -- but this erroneously adds a dot, so TPQ needs to be >=8. 
  // Hmm seems ok now
  REQUIRE(n(60, 0, 1, 4).ToString() == "<qq> c4"); // s/b "<qq> 60"

  // Limit of small note values: demisemiquaver IS recognised.
  // Duration is 1/8 of a crotchet, should be "qqq"
  REQUIRE(n(60, 0, 1, 8).ToString() == "<qqq> c4"); 

  // Smaller than smallest value:
  // Duration is 1/16 of a crotchet, should be "qqqq"
  // Not supported.
  REQUIRE(n(60, 0, 1, 16).ToString() == "<qqq> c4"); // s/b "<qqqq> 60" ?
}

TEST_CASE("BAD Output strings 3", "[Events]")
{
  // Limit of large note values
  // Above a dotted sb4, note values are not recognised.
  // Duration is 32 crotchets (128/4), 8 semibreves.
  REQUIRE(n(60, 0, 128, 4).ToString() == "<sb4.> c4"); // s/b "<sb8> 60" ?
}

TEST_CASE("Set time val", "[Events]")
{
  // Check time val is set correctly, and duration/end set to proper
  //  values, when the duration from the raw events is not an expected
  //  multiple of tpq.

  // n(pitch, start, duration, tpq) 
  const int pitch = 60;
  const int start = 0;
  const int tpq = 480;
  
  // These are real life values, always under the expected tpq multiple.
  // Don't know what the reason is behind this.
  {
  auto e = n(pitch, start, 911, tpq); // Calls SetTimeVal
  REQUIRE(e.m_timeVal == TimeVal::MINIM);
  REQUIRE(e.m_duration == 960);
  REQUIRE(e.m_end == 960);
  }

  {
  auto e = n(pitch, start, 455, tpq); 
  REQUIRE(e.m_timeVal == TimeVal::CROTCHET);
  REQUIRE(e.m_duration == 480);
  REQUIRE(e.m_end == 480);
  }

  {
  auto e = n(pitch, start, 239, tpq); 
  REQUIRE(e.m_timeVal == TimeVal::QUAVER);
  REQUIRE(e.m_duration == 240);
  REQUIRE(e.m_end == 240);
  }

  // So presumably you could get a raw duration that is a bit more than
  //  a tpq multiple.
  {
  auto e = n(pitch, start, 970, tpq); 
  REQUIRE(e.m_timeVal == TimeVal::MINIM);
  REQUIRE(e.m_duration == 960);
  REQUIRE(e.m_end == 960);
  }

  {
  auto e = n(pitch, start, 490, tpq);
  REQUIRE(e.m_timeVal == TimeVal::CROTCHET);
  REQUIRE(e.m_duration == 480);
  REQUIRE(e.m_end == 480);
  }

  {
  auto e = n(pitch, start, 250, tpq); 
  REQUIRE(e.m_timeVal == TimeVal::QUAVER);
  REQUIRE(e.m_duration == 240);
  REQUIRE(e.m_end == 240);
  }
}

TEST_CASE("Append notes to events, split note if required", "[Events]")
{
  // As we add notes from the midi file, we split them: some note lengths
  //  cannot be represented by one time value. 
  // We need to also take bar length into account, so the split is not
  //  problematic when we add bar lines. (We don't re-join split notes,
  //  so splits need to be in the right place.)

  const int tpq = 8;

  SECTION("split notes fit in one bar")
  {
    Events events;
    Event e;
    // 3.5 (7/2) tpqs: = m + c + q   (m is 2, m. is 3, q is 0.5)
    // Result: m. q
    e.m_end = e.m_duration = tpq * 7 / 2;
    AppendNoteEventToEvents(tpq, e, events, TimeSig::TS_4_4);
    REQUIRE(events.size() == 3);
    REQUIRE(events[0].IsNote());
    REQUIRE(events[0].m_timeVal == TimeVal::MINIM);
    REQUIRE(events[0].m_dots == 1);
    REQUIRE(events[0].m_duration == tpq * 3); // dotted minim = 3 crotchets
    REQUIRE(events[0].m_end == tpq * 3); // dotted minim = 3 crotchets
    REQUIRE(events[1].IsTie());
    REQUIRE(events[1].m_start == tpq * 3);
    REQUIRE(events[2].IsNote());
    REQUIRE(events[2].m_timeVal == TimeVal::QUAVER);
    REQUIRE(events[2].m_start == tpq * 3);
    REQUIRE(events[2].m_duration == tpq / 2); 
    REQUIRE(events[2].m_end == tpq * 3 + tpq / 2); 

    // The notes should not overlap, so there should not be any chord
    //  markers. (This was going wrong before as ties and notes were
    //  incorrectly identified as a chord.)
    Events eventsBeforeChord(events);
    InsertChordMarkers(events);
    REQUIRE(eventsBeforeChord.size() == events.size());
  }

  SECTION("split notes don't fit in one bar")
  {
    Events events;
    Event e;
    // 3.5 again but start 1 crotchet before notional bar line
    // 3.5 (7/2) tpqs: = m + c + q   (m is 2, m. is 3, q is 0.5)
    // Result: c t [notional bar line] m t q
    e.m_start = 3 * tpq;
    e.m_duration = tpq * 7 / 2;
    AppendNoteEventToEvents(tpq, e, events, TimeSig::TS_4_4);
    REQUIRE(events.size() == 5);
    REQUIRE(OutputNoteDurations(events) == "c t m t q");

    // The notes should not overlap, so there should not be any chord
    //  markers. (This was going wrong before as ties and notes were
    //  incorrectly identified as a chord.)
    Events eventsBeforeChord(events);
    InsertChordMarkers(events);
    REQUIRE(eventsBeforeChord.size() == events.size());
  }

  SECTION("Note duration 2.5 fits in bar")
  {
    Events events;
    Event e;
    // 2.5 (5/2) tpqs: = m + q   (m is 2, q is 0.5)
    // Result: m t q
    e.m_end = e.m_duration = tpq * 5 / 2;
    AppendNoteEventToEvents(tpq, e, events, TimeSig::TS_4_4);
    REQUIRE(OutputNoteDurations(events) == "m t q");
  }

  SECTION("Note duration 2.5 doesn't fit in bar, 2/4, starting at 1")
  {
    Events events;
    Event e;
    // 2.5 (5/2) tpqs: = m + q   (m is 2, q is 0.5)
    // Can't be m t q because bar line is half way through the minim.
    // Result: c t [|] c t q  OR c t c.  if we don't split on beats.
    e.m_start = 1 * tpq;
    e.m_duration = tpq * 5 / 2;
    AppendNoteEventToEvents(tpq, e, events, TimeSig::TS_2_4);
    REQUIRE(OutputNoteDurations(events) == "c t c.");
  }

  SECTION("Note duration 2.5 doesn't fit in bar, 3/4")
  {
    Events events;
    Event e;
    // 2.5 (5/2) tpqs: = m + q   (m is 2, q is 0.5)
    // Result: c t [|] c t q -- oh, actually it's:  c t c.  - dotted crotchet works.
    e.m_start = 2 * tpq;
    e.m_duration = tpq * 5 / 2;
    AppendNoteEventToEvents(tpq, e, events, TimeSig::TS_3_4);
    REQUIRE(OutputNoteDurations(events) == "c t c.");
  }

  SECTION("Note doesn't fit: 4/4")
  {
    Events events;
    Event e;
    // 6.5 tpqs: = sb + m + q   (sb is 4, m is 2, q is 0.5)
    // Result: sb t m t q
    e.m_end = e.m_duration = tpq * 13 / 2;
    AppendNoteEventToEvents(tpq, e, events, TimeSig::TS_4_4);
    REQUIRE(OutputNoteDurations(events) == "sb t m t q");
  }

  SECTION("Note doesn't fit: 3/4")
  {
    Events events;
    Event e;
    // 6.5 tpqs: = sb + m + q 
    // Result: m. t m. t q
    e.m_end = e.m_duration = tpq * 13 / 2;
    AppendNoteEventToEvents(tpq, e, events, TimeSig::TS_3_4);
    REQUIRE(OutputNoteDurations(events) == "m. t m. t q");
  }

  SECTION("Note doesn't fit: 3/4, 5 crotchets")
  {
    Events events;
    Event e;
    // 5 crotchets in 3/4, should be m. t m
    e.m_end = e.m_duration = tpq * 5;
    AppendNoteEventToEvents(tpq, e, events, TimeSig::TS_3_4);
    REQUIRE(OutputNoteDurations(events) == "m. t m");
  }

  SECTION("Note doesn't fit: 2/4")
  {
    Events events;
    Event e;
    // 6.5 tpqs: = sb + m + q 
    // Result: m t m t m t q
    e.m_end = e.m_duration = tpq * 13 / 2;
    AppendNoteEventToEvents(tpq, e, events, TimeSig::TS_2_4);
    REQUIRE(OutputNoteDurations(events) == "m t m t m t q");
  }
}

TEST_CASE("Notes split on beats", "[Events]")
{ 
  // Notes should be split so that the duration is reduced until it is a 
  //  multiple of the start time. Then we repeat for the rest of the note
  //  duration, adding ties.

  const int tpq = 100; // ticks per quarter note
  Events events;

  // pitch, start, duration, tpq
  auto event1 = n(60, 0,       tpq / 2, tpq); // q
  auto event2 = n(62, tpq / 2, tpq * 2, tpq); // m

  // We need to go through this function to split the notes.
  // This is what gets called for midi events.
  const bool yesSplitOnBeats = true;
  AppendNoteEventToEvents(tpq, event1, events, TimeSig::TS_4_4, yesSplitOnBeats);
  AppendNoteEventToEvents(tpq, event2, events, TimeSig::TS_4_4, yesSplitOnBeats);

//std::cout << OutputEvents(events);
  // <q> 60 62 t <c> 62 t <q> 62 
  //     0  1  2     3  4     5
  //  -- minim is split and tied.

  REQUIRE(events.size() == 6);
  REQUIRE(events[0].IsNote());  // 60
  REQUIRE(events[1].IsNote());  // 62
  REQUIRE(events[2].IsTie());  
  REQUIRE(events[3].IsNote());  
  REQUIRE(events[4].IsTie());  
  REQUIRE(events[5].IsNote());  
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

  InsertRests(tpq, events, TimeSig::TS_4_4);

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
  InsertBarLines(tpq, TimeSig::TS_4_4, events, 0, 0);
  InsertRests(tpq, events, TimeSig::TS_4_4);

  //std::cout << OutputEvents(events);
  // NOT: cr <c> 60 mr | cr <m> 62 cr | mr. <c> 64 | <c> 65 mr. | 
  // Because we now don't add dotted rests, except in compound time sigs.
  // Output is:
  // <c> r 60 <m> r | <c> r <m> 62 <c> r | <m> r <c> r 64 | <c> 65 r <m> r | 
  //     0 1      2 3     4     5      6 7     8     9 10 11    12 13   14 15

  REQUIRE(events.size() == 16);
  REQUIRE(events[0].IsRest());
  REQUIRE(events[1].IsNote());
  REQUIRE(events[2].IsRest());
  REQUIRE(events[3].IsBarLine());
  REQUIRE(events[4].IsRest());
  REQUIRE(events[5].IsNote());
  REQUIRE(events[6].IsRest());
  REQUIRE(events[7].IsBarLine());
  REQUIRE(events[8].IsRest());
  REQUIRE(events[9].IsRest());
  REQUIRE(events[10].IsNote());
  REQUIRE(events[11].IsBarLine());
  REQUIRE(events[12].IsNote());
  REQUIRE(events[13].IsRest());
  REQUIRE(events[14].IsRest());
  REQUIRE(events[15].IsBarLine());
}

TEST_CASE("Insert rests, split on beats", "[Events]")
{
  // Rests are split on beats, so the order is from smallest 
  //  to largest duration.
  const int tpq = 32; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0, tpq / 8, tpq), // qqq at start of bar
    // gap here! 3.75 crotchets long...
  };
  Event barline;
  barline.m_type = EventType::BARLINE;
  barline.m_start = 4 * tpq;
  barline.m_end = barline.m_start;
  events.emplace_back(barline);

  // Expect: <qqq> 60 |

  InsertRests(tpq, events, TimeSig::TS_4_4);
 
//  std::cout << OutputEvents(events);
  // Output is:
  // <qqq> 60 r <qq> r <q> r <c> r <m> r | 
  //       0 1       2     3     4     5 6
  //  -- no dotted rests because not a compound time sig.

  REQUIRE(events.size() == 7); 
  REQUIRE(events[0].IsNote()); // unchanged 
  REQUIRE(events[1].IsRest());
  REQUIRE(events[1].m_timeVal == TimeVal::QQQ);
  REQUIRE(events[1].m_dots == 0);
  REQUIRE(events[2].IsRest());
  REQUIRE(events[2].m_timeVal == TimeVal::SEMIQUAVER);
  REQUIRE(events[2].m_dots == 0);
  REQUIRE(events[3].IsRest());
  REQUIRE(events[3].m_timeVal == TimeVal::QUAVER);
  REQUIRE(events[3].m_dots == 0);
  REQUIRE(events[4].IsRest());
  REQUIRE(events[4].m_timeVal == TimeVal::CROTCHET);
  REQUIRE(events[4].m_dots == 0);
  REQUIRE(events[5].IsRest());
  REQUIRE(events[5].m_timeVal == TimeVal::MINIM);
  REQUIRE(events[5].m_dots == 0);
  REQUIRE(events[6].IsBarLine());
}

TEST_CASE("Reverse", "[Events]")
{
  // Test helper function: reverse generated rest or note events.
  const int tpq = 32; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0,       1 * tpq, tpq), 
    n(61, 1 * tpq, 1 * tpq, tpq), 
    n(62, 2 * tpq, 1 * tpq, tpq),  
    n(63, 3 * tpq, 1 * tpq, tpq)
  };

  Reverse(events);

  REQUIRE(events.size() == 4);
  REQUIRE(events[0].m_start == 0);
  REQUIRE(events[0].m_end == 1 * tpq);
  REQUIRE(events[1].m_start == 1 * tpq);
  REQUIRE(events[1].m_end == 2 * tpq);
  REQUIRE(events[2].m_start == 2 * tpq);
  REQUIRE(events[2].m_end == 3 * tpq);
  REQUIRE(events[3].m_start == 3 * tpq);
  REQUIRE(events[3].m_end == 4 * tpq);
}

TEST_CASE("Insert whole bar rest, not split", "[Events]")
{
  const int tpq = 32; // ticks per quarter note
  Events events
  {
    barline(3 * tpq, tpq),
  };

  InsertRests(tpq, events, TimeSig::TS_3_4);

  std::cout << OutputEvents(events);
  // Expect dotted minim whole bar rest
  REQUIRE(events.size() == 2);
  REQUIRE(events[0].IsRest());
  REQUIRE(events[0].m_timeVal == TimeVal::MINIM);
  REQUIRE(events[0].m_dots == 1);
  REQUIRE(events[0].m_isWholeBar);
  REQUIRE(events[1].IsBarLine());
}

TEST_CASE("Insert rest, duration means it must be split", "[Events]")
{
  // Some durations are not expressible as a [dotted] TimeVal, and must
  //  be split. Also we split on beats.
  // In this test, there is one c, then another qqq at the end of 4 beats.
  // So to fill the gap, with dots, the rest could be 
  //  m + q + qq. OR m + q. + qqq
  // With dots turned off, the result is
  //  m + q + qq + qqq
  const int tpq = 32; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0, 1 * tpq, tpq), // c 
    // gap here! 2.75 crotchets long
    n(64, tpq * 31 / 8, tpq / 8, tpq), // qqq at end of 4/4 bar
  };
  //  4/4 c ...  qqq |

  InsertRests(tpq, events, TimeSig::TS_4_4);
 
  // Expect rests: c c  q qq qqq (no dots, not compound time sig)
  //std::cout << OutputEvents(events);

  REQUIRE(events.size() == 7); // 5 rests added
  REQUIRE(events[0].IsNote()); // unchanged 
  REQUIRE(events[1].IsRest());
  REQUIRE(events[1].m_timeVal == TimeVal::CROTCHET);
  REQUIRE(events[1].m_dots == 0);
  REQUIRE(events[2].IsRest());
  REQUIRE(events[2].m_timeVal == TimeVal::CROTCHET);
  REQUIRE(events[2].m_dots == 0);
  REQUIRE(events[3].IsRest());
  REQUIRE(events[3].m_timeVal == TimeVal::QUAVER);
  REQUIRE(events[3].m_dots == 0);
  REQUIRE(events[4].IsRest());
  REQUIRE(events[4].m_timeVal == TimeVal::SEMIQUAVER);
  REQUIRE(events[4].m_dots == 0);
  REQUIRE(events[5].IsRest());
  REQUIRE(events[5].m_timeVal == TimeVal::QQQ);
  REQUIRE(events[5].m_dots == 0);
  REQUIRE(events[6].IsNote()); // unchanged 
}

TEST_CASE("Add bar lines 3/4, with anacrusis c", "[Events]")
{
  const int tpq = 256; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0 * tpq, 1 * tpq, tpq), 
    n(61, 1 * tpq, 1 * tpq, tpq),  
    n(62, 2 * tpq, 1 * tpq, tpq),  
    n(63, 3 * tpq, 1 * tpq, tpq),  
    n(64, 4 * tpq, 1 * tpq, tpq),  
  };

  InsertBarLines(tpq, TimeSig::TS_3_4, events, 0, 1 * tpq); // anac. length  c
  InsertRests(tpq, events, TimeSig::TS_3_4);

  // TODO: Why do we have to restate <c> ??
  // Also, TODO trim final space.
  // NB rest is split because of 3/4 rest-splitting rules?
  REQUIRE(OutputEvents(events) == "<c> c4 | c4+ d4 d4+ | e4 r r | ");
}

TEST_CASE("Add bar lines 3/4, with anacrusis m", "[Events]")
{
  const int tpq = 256; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0 * tpq, 1 * tpq, tpq), 
    n(61, 1 * tpq, 1 * tpq, tpq),  
    n(62, 2 * tpq, 1 * tpq, tpq),  
    n(63, 3 * tpq, 1 * tpq, tpq),  
    n(64, 4 * tpq, 1 * tpq, tpq),  
  };

  InsertBarLines(tpq, TimeSig::TS_3_4, events, 0, 2 * tpq); // anac. length m
  InsertRests(tpq, events, TimeSig::TS_3_4);

  // TODO: Why do we have to restate <c> ??
  // Also, TODO trim final space.
  REQUIRE(OutputEvents(events) == "<c> c4 c4+ | d4 d4+ e4 | ");
}

TEST_CASE("Add bar lines 4/4, with anacrusis c", "[Events]")
{
  const int tpq = 256; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0 * tpq, 1 * tpq, tpq), 
    n(61, 1 * tpq, 1 * tpq, tpq),  
    n(62, 2 * tpq, 1 * tpq, tpq),  
    n(63, 3 * tpq, 1 * tpq, tpq),  
    n(64, 4 * tpq, 1 * tpq, tpq),  
  };

  InsertBarLines(tpq, TimeSig::TS_4_4, events, 0, 1 * tpq); // anac. length  c
  InsertRests(tpq, events, TimeSig::TS_4_4);

  // TODO: Why do we have to restate <c> ??
  // Also, TODO trim final space.
  REQUIRE(OutputEvents(events) == "<c> c4 | c4+ d4 d4+ e4 | ");
}

TEST_CASE("Add bar lines 4/4, with anacrusis m", "[Events]")
{
  const int tpq = 256; // ticks per quarter note
  Events events
  { 
    // pitch, start, duration, tpq
    n(60, 0 * tpq, 1 * tpq, tpq), 
    n(61, 1 * tpq, 1 * tpq, tpq),  
    n(62, 2 * tpq, 1 * tpq, tpq),  
    n(63, 3 * tpq, 1 * tpq, tpq),  
    n(64, 4 * tpq, 1 * tpq, tpq),  
  };

  InsertBarLines(tpq, TimeSig::TS_4_4, events, 0, 2 * tpq); // anac. length m 
  InsertRests(tpq, events, TimeSig::TS_4_4);
  REQUIRE(OutputEvents(events) == "<c> c4 c4+ | d4 d4+ e4 r | ");
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

  InsertBarLines(tpq, TimeSig::TS_4_4, events, 0, 0);

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

  InsertBarLines(tpq, TimeSig::TS_3_4, events, 0, 0);
  // Same note events as in the 4/4 test, but now 
  //  the notes don't fall nicely within bars - add rests to show
  //  the timing, or it's hard to make sense of the result.
  InsertRests(tpq, events, TimeSig::TS_3_4);

//std::cout << OutputEvents(events);
  // Each bar is 3 crotchets duration; notes are split and tied across bar lines.
  // Final rest is split, because as a minim, the start is not a multiple of
  //  its duration.
  // <m.> 60 | t <c> 60 <m> 62 | t <c> 62 r  64 | t <c> 64 <m> r | <c> 65 r r | 
  REQUIRE(events.size() == 19);
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
  REQUIRE(events[17].IsRest());
  REQUIRE(events[18].IsBarLine());
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

  InsertBarLines(tpq, TimeSig::TS_4_4, events, 0, 0);
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

  InsertBarLines(tpq, TimeSig::TS_4_4, events, 0, 0);
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
  InsertBarLines(tpq, TimeSig::TS_4_4, events, 0, 0);

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
  InsertBarLines(tpq, TimeSig::TS_4_4, events, 0, 0);

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
  InsertBarLines(tpq, TimeSig::TS_4_4, events, 0, 0);

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
  InsertBarLines(tpq, TimeSig::TS_4_4, events, 0, 0);
  InsertRests(tpq, events, TimeSig::TS_4_4);
  InsertTimeSetEvents(tpq, events); // need time set event to correctly place final note

  auto str = OutputEvents(events);
  //std::cout << str;
  // Expect time set event to place final note; time set value is in crotchets
  const std::string expected = "( <sb> c4 <m> e4 ) time 3 <c> d4";
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
  InsertBarLines(tpq, TimeSig::TS_4_4, events, 0, 0);
  InsertRests(tpq, events, TimeSig::TS_4_4);
  InsertTimeSetEvents(tpq, events);

  auto str = OutputEvents(events);
  //std::cout << str;
  // Expect time set event to correctly place the 3rd note.
  const std::string expected = "( <sb> c4 <m> e4 ) time 2 <c> d4"; 
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
  InsertBarLines(tpq, TimeSig::TS_4_4, events, 0, 0);
  InsertRests(tpq, events, TimeSig::TS_4_4);
  InsertTimeSetEvents(tpq, events);

  auto str = OutputEvents(events);
  //std::cout << str;
  // Time set value is in crotchets from start of piece
  const std::string expected = "<sb> c4 time 2 <c> d4";
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
  InsertBarLines(tpq, TimeSig::TS_4_4, events, 0, 0);
  InsertRests(tpq, events, TimeSig::TS_4_4);
  InsertTimeSetEvents(tpq, events);

  auto str = OutputEvents(events);
  //std::cout << str;
  // Time set value is in crotchets, so we expect time set of 0.5 
  const std::string expected = "<sb> c4 time 0.5 <q> d4";
  REQUIRE(str.substr(0, expected.size()) == expected);
}

