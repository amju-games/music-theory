#include "catch.hpp"
#include "NoteRun.h"

using namespace Amju;

TEST_CASE("Sign runs 1", "[NoteRun]")
{
  std::vector<int> diffs =    { 0, 0, 0, 0 };
  std::vector<int> expected = { 0, 0, 0, 0 };

  // Count contiguous runs of the same sign in the input
  auto signRuns = CountSignRuns(diffs);
  REQUIRE(signRuns == expected); 
}

TEST_CASE("Sign runs 2", "[NoteRun]")
{
  std::vector<int> diffs =    { 1, 1, 1, 1 };
  std::vector<int> expected = { 0, 1, 2, 3 };

  // Count contiguous runs of the same sign in the input
  auto signRuns = CountSignRuns(diffs);
  REQUIRE(signRuns == expected); 
}

TEST_CASE("Sign runs 3", "[NoteRun]")
{
  std::vector<int> diffs =    { 0, 1, 1, 1 };
  std::vector<int> expected = { 0, 1, 2, 3 };

  // Count contiguous runs of the same sign in the input
  auto signRuns = CountSignRuns(diffs);
  REQUIRE(signRuns == expected); 
}

TEST_CASE("Sign runs 4", "[NoteRun]")
{
  std::vector<int> diffs =    { 0, 1, 0, 1 };
  std::vector<int> expected = { 0, 1, 0, 1 };

  // Count contiguous runs of the same sign in the input
  auto signRuns = CountSignRuns(diffs);
  REQUIRE(signRuns == expected); 
}

TEST_CASE("Sign runs", "[NoteRun]")
{
  std::vector<int> diffs =    { 0, 1, -4, -2, 0, 0, 3, 5, 0 };
  std::vector<int> expected = { 0, 1,  1,  2, 0, 0, 1, 2, 0 };

  // Count contiguous runs of the same sign in the input
  auto signRuns = CountSignRuns(diffs);
  REQUIRE(signRuns == expected); 
}

TEST_CASE("Diffs", "[NoteRun]")
{
  NoteEvents noteEvents
  {
    // pitch, time, type, coord
    NoteEvent(60, 0.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(61, 1.f,  NoteEventType::NOTE_ON,  {0, 0}), // run start
    NoteEvent(60, 2.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(59, 3.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(63, 4.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(70, 5.f,  NoteEventType::NOTE_ON,  {0, 0}), // run start
    NoteEvent(72, 6.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(73, 7.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(60, 8.f,  NoteEventType::NOTE_ON,  {0, 0}),
  };

  auto diffs = FindNoteDiffs(noteEvents);
  REQUIRE(diffs.size() == noteEvents.size());
  // Expected diffs are zero where they are too big to count as a run.
  std::vector<int> expected = { 0, 1, -1, -1, 0, 0, 2, 1, 0 };
  REQUIRE(diffs == expected);
}

TEST_CASE("1 note", "[NoteRun]")
{
  // The events should be in time order.
  NoteEvents noteEvents
  {
    // pitch, time, type, coord
    NoteEvent(60, 0.f,  NoteEventType::NOTE_ON,  {0, 0}),
  };

  auto runs = FindNoteRuns(noteEvents);

  REQUIRE(runs.size() == 0);
}

TEST_CASE("2 notes", "[NoteRun]")
{
  // The events should be in time order.
  NoteEvents noteEvents
  {
    // pitch, time, type, coord
    NoteEvent(60, 0.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(61, 1.f,  NoteEventType::NOTE_ON,  {0, 0}),
  };

  auto runs = FindNoteRuns(noteEvents);

  REQUIRE(runs.size() == 0);
}

TEST_CASE("3 note run", "[NoteRun]")
{
  // The events should be in time order.
  NoteEvents noteEvents
  {
    // pitch, time, type, coord
    NoteEvent(60, 0.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(61, 1.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(62, 2.f,  NoteEventType::NOTE_ON,  {0, 0}),
  };

  auto runs = FindNoteRuns(noteEvents);

  REQUIRE(runs.size() == 1);
  REQUIRE(runs.front().m_start == 0);
  REQUIRE(runs.front().m_finish == 3);
  REQUIRE(runs.front().m_isUp == true);
}

TEST_CASE("3 note run in 4 notes", "[NoteRun]")
{
  // The events should be in time order.
  NoteEvents noteEvents
  {
    // pitch, time, type, coord
    NoteEvent(61, 0.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(60, 1.f,  NoteEventType::NOTE_ON,  {0, 0}), // run start
    NoteEvent(61, 2.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(62, 3.f,  NoteEventType::NOTE_ON,  {0, 0}),
  };

  auto runs = FindNoteRuns(noteEvents);

  REQUIRE(runs.size() == 1);
  REQUIRE(runs.front().m_start == 1);
  REQUIRE(runs.front().m_finish == 4);
  REQUIRE(runs.front().m_isUp == true);
}

TEST_CASE("4 note run in 6 notes, ends before notes end", "[NoteRun]")
{
  // The events should be in time order.
  NoteEvents noteEvents
  {
    // pitch, time, type, coord
    NoteEvent(61, 0.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(60, 1.f,  NoteEventType::NOTE_ON,  {0, 0}), // run start
    NoteEvent(61, 2.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(62, 3.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(63, 4.f,  NoteEventType::NOTE_ON,  {0, 0}),
    // This jump is ok if we allow 3 semitone jumps but the sign is different.
    NoteEvent(60, 5.f,  NoteEventType::NOTE_ON,  {0, 0}),
  };

  auto runs = FindNoteRuns(noteEvents);

  REQUIRE(runs.size() == 1);
  REQUIRE(runs.front().m_start == 1);
  REQUIRE(runs.front().m_finish == 5);
  REQUIRE(runs.front().m_isUp == true);
}

TEST_CASE("2 runs", "[NoteRun]")
{
  // The events should be in time order.
  NoteEvents noteEvents
  {
    // pitch, time, type, coord
    NoteEvent(60, 0.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(61, 1.f,  NoteEventType::NOTE_ON,  {0, 0}), // run start
    NoteEvent(60, 2.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(59, 3.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(63, 4.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(70, 5.f,  NoteEventType::NOTE_ON,  {0, 0}), // run start
    NoteEvent(72, 6.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(73, 7.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(60, 8.f,  NoteEventType::NOTE_ON,  {0, 0}),
  };

  auto runs = FindNoteRuns(noteEvents);

  REQUIRE(runs.size() == 2);
  REQUIRE(runs[0].m_start == 1);
  REQUIRE(runs[0].m_finish == 4);
  REQUIRE(runs[0].m_isUp == false);
  REQUIRE(runs[1].m_start == 5);
  REQUIRE(runs[1].m_finish == 8);
  REQUIRE(runs[1].m_isUp == true);
}

