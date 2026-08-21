#include "catch.hpp"
#include "Resumer.h"

using namespace Amju;

// More tests, try to track down Resumer bug, sigh.
TEST_CASE("Resumer::FindNoteEventAtTime", "[Resumer][NoteEvents]") {

    SECTION("Returns false for an empty note vector") {
        NoteEvents emptyEvents;
        REQUIRE_FALSE(Resumer::FindNoteEventAtTime(1.0f, emptyEvents, 0.01f));
    }

    SECTION("Returns false for negative epsilon") {
        NoteEvents events = {{1.0f}};
        REQUIRE_FALSE(Resumer::FindNoteEventAtTime(1.0f, events, -0.01f));
    }

    SECTION("Finds exact match") {
        NoteEvents events = {{0.5f}, {1.0f}, {2.0f}};
        REQUIRE(Resumer::FindNoteEventAtTime(1.0f, events, 0.001f));
    }

    SECTION("Finds event within epsilon boundary") {
        NoteEvents events = {{1.0005f}};
        const float targetTime = 1.0f;
        const float epsilon = 0.001f;

        REQUIRE(Resumer::FindNoteEventAtTime(targetTime, events, epsilon));
    }

    SECTION("Fails to find event outside epsilon boundary") {
        NoteEvents events = {{1.002f}};
        const float targetTime = 1.0f;
        const float epsilon = 0.001f;

        REQUIRE_FALSE(Resumer::FindNoteEventAtTime(targetTime, events, epsilon));
    }

    SECTION("Handles boundaries correctly (inclusive epsilon)") {
        NoteEvents events = {{1.005f}};
        
        // Exactly on upper boundary: |1.005 - 1.000| == 0.005
        REQUIRE(Resumer::FindNoteEventAtTime(1.0f, events, 0.005f));
        
        // Just outside upper boundary
        REQUIRE_FALSE(Resumer::FindNoteEventAtTime(1.0f, events, 0.0049f));
    }

    SECTION("Correctly handles multiple adjacent events") {
        NoteEvents events = {{0.1f}, {0.49f}, {0.52f}, {0.9f}};

        // Target: 0.50, Epsilon: 0.02 -> Matches 0.49 and 0.52
        REQUIRE(Resumer::FindNoteEventAtTime(0.50f, events, 0.02f));
        
        // Target: 0.50, Epsilon: 0.005 -> No match
        REQUIRE_FALSE(Resumer::FindNoteEventAtTime(0.50f, events, 0.005f));
    }
}

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

  const float epsilon = 0.00001f;
  REQUIRE(Resumer::FindNoteEventAtTime(0.f,     noteEvents, epsilon) == false);
  REQUIRE(Resumer::FindNoteEventAtTime(1.f,     noteEvents, epsilon) == true);
  REQUIRE(Resumer::FindNoteEventAtTime(2.0001f, noteEvents, epsilon) == false);
  REQUIRE(Resumer::FindNoteEventAtTime(5.f,     noteEvents, epsilon) == true);
  REQUIRE(Resumer::FindNoteEventAtTime(7.3f,    noteEvents, epsilon) == false);
  REQUIRE(Resumer::FindNoteEventAtTime(100.f,   noteEvents, epsilon) == false);
}

TEST_CASE("Find note event at time: empty notes", "[Resumer]")
{
  NoteEvents noteEvents
  {
  };

  const float epsilon = 0.00001f;
  REQUIRE(Resumer::FindNoteEventAtTime(0,     noteEvents, epsilon) == false);
  REQUIRE(Resumer::FindNoteEventAtTime(100.f, noteEvents, epsilon) == false);
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

// More tests for GoToFirstBeatOfBar.
TEST_CASE("Resumer::GoToFirstBeatOfBar", "[Resumer][Beat]") {

    SECTION("Returns 0 for empty beats vector") {
        BeatVec beats;
        //auto iter = beats.end();
        BeatVec::const_iterator iter = beats.end();
        REQUIRE(Resumer::GoToFirstBeatOfBar(beats, iter) == 0.0f);
    }

    SECTION("Returns 0 when iter is beats.end()") {
        BeatVec beats = {{0.0f, 1, 1}, {0.25f, 1, 2}};
        //auto iter = beats.end();
        BeatVec::const_iterator iter = beats.end();
        REQUIRE(Resumer::GoToFirstBeatOfBar(beats, iter) == 0.0f);
    }

    SECTION("Already at first beat of bar does not move iterator") {
        BeatVec beats = {
            {0.0f, 1, 1}, {0.25f, 1, 2}, {0.50f, 1, 3},
            {0.75f, 2, 1}, {1.00f, 2, 2}
        };

        //auto iter = beats.begin() + 3; // Bar 2, Beat 1 (0.75f)
        BeatVec::const_iterator iter = beats.begin() + 3;
        float result = Resumer::GoToFirstBeatOfBar(beats, iter);

        REQUIRE(result == 0.75f);
        REQUIRE(iter == beats.begin() + 3);
    }

    SECTION("Rewinds from middle of bar to beat 1") {
        BeatVec beats = {
            {0.0f, 1, 1}, {0.25f, 1, 2}, {0.50f, 1, 3},
            {0.75f, 2, 1}, {1.00f, 2, 2}, {1.25f, 2, 3}
        };

        //auto iter = beats.begin() + 5; // Bar 2, Beat 3 (1.25f)
        BeatVec::const_iterator iter = beats.begin() + 5;
        float result = Resumer::GoToFirstBeatOfBar(beats, iter);

        REQUIRE(result == 0.75f); // Time for Bar 2, Beat 1
        REQUIRE(iter->m_bar == 2);
        REQUIRE(iter->m_beat == 1);
    }

    SECTION("Handles vector starting mid-bar (iter at beats.begin() with beat > 1)") {
        // Partial slice starting at Beat 3
        BeatVec beats = {
            {0.50f, 1, 3}, {0.75f, 1, 4}, {1.00f, 2, 1}
        };

        //auto iter = beats.begin(); // Bar 1, Beat 3
        BeatVec::const_iterator iter = beats.begin();
        float result = Resumer::GoToFirstBeatOfBar(beats, iter);

        // Stops safely at beats.begin() without decrementing past boundary
        REQUIRE(result == 0.50f);
        REQUIRE(iter == beats.begin());
    }
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

TEST_CASE("Resumer::FindResumePoint", "[Resumer][ResumePoint]") {

    const float END_OF_SONG = 3600.f;

    // Setup typical song structure: 2 bars, 4 beats per bar
    // Bar 1: t = 0.0, 0.25, 0.50, 0.75
    // Bar 2: t = 1.0, 1.25, 1.50, 1.75
    const BeatVec standardBeats = {
        {0.00f, 1, 1}, {0.25f, 1, 2}, {0.50f, 1, 3}, {0.75f, 1, 4},
        {1.00f, 2, 1}, {1.25f, 2, 2}, {1.50f, 2, 3}, {1.75f, 2, 4}
    };

    SECTION("Returns pauseTime if beats vector is empty") {
        BeatVec emptyBeats;
        NoteEvents notes = {{0.0f}};
        REQUIRE(Resumer::FindResumePoint(1.2f, emptyBeats, notes) == END_OF_SONG);
    }

    SECTION("Resumes at beat 1 of current bar when note exists at beat 1") {
        NoteEvents notes = {{0.0f}, {1.0f}}; // Notes at start of Bar 1 and Bar 2

        // Paused in middle of Bar 2 (t = 1.3f)
        float resumeTime = Resumer::FindResumePoint(1.3f, standardBeats, notes);
        REQUIRE(resumeTime == 1.0f); // Beat 1 of Bar 2
    }

    SECTION("Rewinds to previous bar if current bar beat 1 has no note event") {
        // Note exists at Bar 1 Beat 1 (0.0f), but Bar 2 Beat 1 (1.0f) has NO note
        NoteEvents notes = {{0.0f}, {1.25f}};

        // Paused in middle of Bar 2 (t = 1.5f)
        float resumeTime = Resumer::FindResumePoint(1.5f, standardBeats, notes);
        REQUIRE(resumeTime == 0.0f); // Rewinds past Bar 2 back to Bar 1 Beat 1
    }

    SECTION("Handles pause point prior to the first beat") {
        NoteEvents notes = {{0.0f}};
        
        // Paused before beat sequence starts
        // We return 0, meaning restart the song.
        float resumeTime = Resumer::FindResumePoint(-0.5f, standardBeats, notes);
        REQUIRE(resumeTime == 0);
    }

    SECTION("Falls back to start of song if no bars have a starting note") {
        NoteEvents notes = {{0.5f}}; // Notes only in middle of bars

        // Paused in Bar 2
        float resumeTime = Resumer::FindResumePoint(1.5f, standardBeats, notes);
        REQUIRE(resumeTime == 0.0f); // Stops at beats.begin()
    }
}

