#include "catch.hpp"
#include "Resumer.h"

using namespace Amju;

TEST_CASE("Find note event at time", "[Resumer]")
{
  // The events should be in time order.
  NoteEvents noteEvents
  {
    // pitch, time, type, coord
    NoteEvent(60, 0.5f, NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(61, 1.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(60, 2.f,  NoteEventType::NOTE_OFF, {0, 0}),
    NoteEvent(62, 3.5f, NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(61, 4.f,  NoteEventType::NOTE_OFF, {0, 0}),
    NoteEvent(62, 5.f,  NoteEventType::NOTE_OFF, {0, 0}),
  };

  REQUIRE(Resumer::FindNoteEventAtTime(0.f,     noteEvents) == false);
  REQUIRE(Resumer::FindNoteEventAtTime(1.f,     noteEvents) == true);
  REQUIRE(Resumer::FindNoteEventAtTime(2.0001f, noteEvents) == false);
  REQUIRE(Resumer::FindNoteEventAtTime(5.f,     noteEvents) == true);
  REQUIRE(Resumer::FindNoteEventAtTime(7.3f,    noteEvents) == false);
  REQUIRE(Resumer::FindNoteEventAtTime(100.f,   noteEvents) == false);
}

TEST_CASE("Find note event at time: empty notes", "[Resumer]")
{
  NoteEvents noteEvents
  {
  };

  REQUIRE(Resumer::FindNoteEventAtTime(0,     noteEvents) == false);
  REQUIRE(Resumer::FindNoteEventAtTime(100.f, noteEvents) == false);
}

TEST_CASE("NextBeatAfterTime", "[Resumer]")
{
  const BeatVec beats
  {
    // Time, bar (1-based), beat (1-based)
    // 4/4
    { 0.f, 1, 1 },
    { 1.f, 1, 2 },
    { 2.f, 1, 3 },
    { 3.f, 1, 4 },
    { 4.f, 2, 1 },
    { 5.f, 2, 2 },
    { 6.f, 2, 3 },
    { 7.f, 2, 4 },
  };

  REQUIRE(Resumer::NextBeatAfterTime(beats, 0.f   )->m_time == 1.f);
  REQUIRE(Resumer::NextBeatAfterTime(beats, 0.001f)->m_time == 1.f);
  REQUIRE(Resumer::NextBeatAfterTime(beats, 0.999f)->m_time == 1.f);
  REQUIRE(Resumer::NextBeatAfterTime(beats, 1.f   )->m_time == 2.f);
  REQUIRE(Resumer::NextBeatAfterTime(beats, 6.99f )->m_time == 7.f);
  REQUIRE(Resumer::NextBeatAfterTime(beats, 7.f   ) == beats.cend());
}

TEST_CASE("Go to first beat of bar: 4/4", "[Resumer]")
{
  const BeatVec beats
  {
    // Time, bar (1-based), beat (1-based)
    // 4/4
    { 0.f, 1, 1 },
    { 1.f, 1, 2 },
    { 2.f, 1, 3 },
    { 3.f, 1, 4 },
    { 4.f, 2, 1 },
    { 5.f, 2, 2 },
    { 6.f, 2, 3 },
    { 7.f, 2, 4 },
  };

  BeatVec::const_iterator it = beats.begin();
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 0.f);
  it = beats.begin() + 1;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 0.f);
  it = beats.begin() + 2;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 0.f);
  it = beats.begin() + 3;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 0.f);
  it = beats.begin() + 4;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 4.f);
  it = beats.begin() + 5;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 4.f);
  it = beats.begin() + 6;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 4.f);
  it = beats.begin() + 7;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 4.f);
  it = beats.begin() + 8;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 0.f); // begin() + 8

  it = beats.end();
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 0.f);
}

TEST_CASE("Go to first beat of bar: 3/4", "[Resumer]")
{
  const BeatVec beats
  {
    // Time, bar (1-based), beat (1-based)
    // 3/4
    { 0.f, 1, 1 },
    { 1.f, 1, 2 },
    { 2.f, 1, 3 },
    { 3.f, 2, 1 },
    { 4.f, 2, 2 },
    { 5.f, 2, 3 },
    { 6.f, 3, 1 },
    { 7.f, 3, 2 },
    { 8.f, 3, 3 },
  };

  auto it = beats.begin();
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 0.f);
  it = beats.begin() + 1;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 0.f);
  it = beats.begin() + 2;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 0.f);
  it = beats.begin() + 3;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 3.f);
  it = beats.begin() + 4;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 3.f);
  it = beats.begin() + 5;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 3.f);
  it = beats.begin() + 6;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 6.f);
  it = beats.begin() + 7;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 6.f);
  it = beats.begin() + 8;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 6.f);
  it = beats.begin() + 9;
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 0.f);
  it = beats.end();
  REQUIRE(Resumer::GoToFirstBeatOfBar(beats, it) == 0.f);
}

