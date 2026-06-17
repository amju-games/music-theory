#include "catch.hpp"
#include "Clef.h"
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
    REQUIRE(changes[0].clef == Clef::TREBLE); // treble wins over bass!
    REQUIRE(changes[0].tick_time == 0);
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
    REQUIRE(changes[0].clef == Clef::BASS);
    REQUIRE(changes[0].tick_time == 0);
    REQUIRE(changes[1].clef == Clef::TREBLE);
    REQUIRE(changes[1].tick_time == 8 * 4 * tpq); // i.e. after 2 chunks
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
    REQUIRE(changes[0].clef == Clef::BASS);
    REQUIRE(changes[0].tick_time == 0);
    REQUIRE(changes[1].clef == Clef::TREBLE);
    REQUIRE(changes[1].tick_time == 8 * tpq); // i.e. after 2 bars
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
  REQUIRE(changes[0].clef == Clef::BASS);
  REQUIRE(changes[0].tick_time == 0);
  REQUIRE(changes[1].clef == Clef::TREBLE);
  REQUIRE(changes[1].tick_time == 8 * tpq + anacrusisTicks); // i.e. after 2 bars + 1 crotchet
}

