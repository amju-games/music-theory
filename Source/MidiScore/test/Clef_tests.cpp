#include "catch.hpp"
#include "Clef.h"
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

Events semibreves(const std::vector<int> pitches, int tpq)
{
  Events events;
  int start = 0;
  for (int p : pitches)
  {
    events.push_back(n(p, start, 4 * tpq, tpq));
    start += 4 * tpq;
  }
  return events;
}

TEST_CASE("Guess clef, bass, no change", "[Clef]")
{
  const int tpq = 480; // arbitrary
  // Add semibreves in different pitches to set up a pitch sequence.
  Events events = semibreves({ 48, 48 }, tpq);

/*
Clef GuessClef(const Events& e, int tpq, int anacrusisTicks, TimeSig ts,
  ClefChanges& allClefChanges, // result: all the changes
  bool justTrebleAndBass = true);
*/
  ClefChanges changes;
  Clef c = GuessClef(events, tpq, 0, TimeSig::TS_4_4, changes);
  REQUIRE(c == Clef::BASS);
}

TEST_CASE("Guess clef, treble, no change", "[Clef]")
{
  const int tpq = 480; // arbitrary
  // Add semibreves in different pitches to set up a pitch sequence.
  Events events = semibreves({ 72, 72 }, tpq);

  ClefChanges changes;
  Clef c = GuessClef(events, tpq, 0, TimeSig::TS_4_4, changes);
  REQUIRE(c == Clef::TREBLE);
  REQUIRE(changes.size() == 1); // no changes
}

TEST_CASE("Guess clef, bass to treble change", "[Clef]")
{
  const int tpq = 1; // arbitrary

  SECTION("Default chunk size and threshold: changes too quick; discarded.")
  {
    // Add semibreves in different pitches to set up a pitch sequence.
    Events events = semibreves({ 48, 48, 72, 72 }, tpq);

    ClefChanges changes;
    Clef c = GuessClef(
      events, tpq, 0, TimeSig::TS_4_4, changes, true);
    REQUIRE(changes.size() == 1);
    REQUIRE(changes[0].m_clef == Clef::TREBLE); // treble wins over bass!
    REQUIRE(changes[0].m_start == 0);
  }

  SECTION("Longer sustained difference: change is triggered.")
  {
    // Add semibreves in different pitches to set up a pitch sequence.
    Events events = semibreves({ 
      48, 48, 48, 48, 
      48, 48, 48, 48, 
      72, 72, 72, 72,
      72, 72, 72, 72,
    }, tpq);

    ClefChanges changes;
    Clef c = GuessClef(
      events, tpq, 0, TimeSig::TS_4_4, changes, true);
    REQUIRE(changes.size() == 2);
    REQUIRE(changes[0].m_clef == Clef::BASS);
    REQUIRE(changes[0].m_start == 0);
    REQUIRE(changes[1].m_clef == Clef::TREBLE);
    REQUIRE(changes[1].m_start == 8 * 4 * tpq); // i.e. after 2 chunks
  }

  SECTION("Non default chunk size and threshold: change is triggered.")
  {
    // Add semibreves in different pitches to set up a pitch sequence.
    Events events = semibreves({ 48, 48, 72, 72 }, tpq);

    const int chunkSize = 2;
    const int threshold = 1;
    ClefChanges changes;
    Clef c = GuessClef(
      events, tpq, 0, TimeSig::TS_4_4, changes, true, chunkSize, threshold);
    REQUIRE(changes.size() == 2);
    REQUIRE(changes[0].m_clef == Clef::BASS);
    REQUIRE(changes[0].m_start == 0);
    REQUIRE(changes[1].m_clef == Clef::TREBLE);
    REQUIRE(changes[1].m_start == 8 * tpq); // i.e. after 2 bars
  }
}

TEST_CASE("Guess clef, bass to treble change, with anacrusis", "[Clef]")
{
  const int tpq = 480; // arbitrary
  // Add semibreves in different pitches to set up a pitch sequence.
  Events events = semibreves({ 48, 48, 72, 72 }, tpq);

  const int chunkSize = 2;
  const int threshold = 1;
  ClefChanges changes;
  int anacrusisTicks = 1 * tpq; // 1 crotchet of anacrusis
  Clef c = GuessClef(events, tpq, anacrusisTicks, TimeSig::TS_4_4, changes, true, chunkSize, threshold);
  REQUIRE(changes.size() == 2);
  REQUIRE(changes[0].m_clef == Clef::BASS);
  REQUIRE(changes[0].m_start == 0);
  REQUIRE(changes[1].m_clef == Clef::TREBLE);
  REQUIRE(changes[1].m_start == 8 * tpq + anacrusisTicks); // i.e. after 2 bars + 1 crotchet
}

TEST_CASE("Interleave clef changes with notes and bar lines", "[Clef]")
{
  const int tpq = 480; // arbitrary
  Events events = semibreves({ 48, 48, 72, 72 }, tpq);
  ClefChanges changes;
  Clef c = GuessClef(
    events, tpq, 0, TimeSig::TS_4_4, changes, true, 2, 1);
 
  const int anacrusis = 0;
  int numBars = 4;
  InsertBarLines(tpq, TimeSig::TS_4_4, events, numBars, anacrusis); 
  REQUIRE(OutputEvents(events) == "<sb> 48 | 48 | 72 | 72 | ");
  InsertClefs(events, changes);
  REQUIRE(OutputEvents(events) == "clef-b <sb> 48 | 48 | clef-t 72 | 72 | ");
}

TEST_CASE("Interleave clef changes, with anacrusis ", "[Clef]")
{
  const int tpq = 480; // arbitrary
  Events events = semibreves({ 48, 48, 72, 72 }, tpq);
  ClefChanges changes;
  Clef c = GuessClef(
    events, tpq, 0, TimeSig::TS_4_4, changes, true, 2, 1);
 
  const int anacrusis = 2 * tpq;
  const int numBars = 5; // extra bar because of anac.
  InsertBarLines(tpq, TimeSig::TS_4_4, events, numBars, anacrusis);  
  REQUIRE(OutputEvents(events) == "<m> 48 | t 48 48 | t 48 72 | t 72 72 | t 72 | ");
  InsertClefs(events, changes);
  // Treble clef change is skipped because it can't fit in the shifted events.
  // It would have to be mid-bar or would have to increase its time to the next
  //  bar. But if we do that, we have to check for ties.
  REQUIRE(OutputEvents(events) == "clef-b <m> 48 | t 48 48 | t 48 72 | t 72 72 | t 72 | ");
}

