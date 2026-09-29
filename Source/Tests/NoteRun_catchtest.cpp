#include "catch.hpp"
#include "NoteRun.h"

using namespace Amju;

// Default values for note runs, just in this file.
static const int MIN_DIFF = 1;
static const int MAX_DIFF = 3;

// For these tests, populate m_id in NoteEvent sequences.
// In normal use this happens in GuiMusicScore when we load 
//  the events.
void PopulateIdsForNoteEvents(NoteEvents& noteEvents)
{
  int id = 0;
  for (auto& ne : noteEvents)
  {
    ne.m_id = id;
    ++id;
  }
}

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
  PopulateIdsForNoteEvents(noteEvents);

  auto diffs = FindNoteDiffs(noteEvents, MIN_DIFF, MAX_DIFF);
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
  PopulateIdsForNoteEvents(noteEvents);

  auto runs = FindNoteRunsNoNoteOffEvents(noteEvents, MIN_DIFF, MAX_DIFF);

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
  PopulateIdsForNoteEvents(noteEvents);

  auto runs = FindNoteRunsNoNoteOffEvents(noteEvents, MIN_DIFF, MAX_DIFF);

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
  PopulateIdsForNoteEvents(noteEvents);

  auto runs = FindNoteRunsNoNoteOffEvents(noteEvents, MIN_DIFF, MAX_DIFF);

  const std::vector<int> expectedIds = { 0, 1, 2 };
  REQUIRE(runs.size() == 1);
  REQUIRE(runs.front().m_ids == expectedIds);
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
  PopulateIdsForNoteEvents(noteEvents);

  auto runs = FindNoteRunsNoNoteOffEvents(noteEvents, MIN_DIFF, MAX_DIFF);

  const std::vector<int> expectedIds = { 1, 2, 3 };
  REQUIRE(runs.size() == 1);
  REQUIRE(runs.front().m_ids == expectedIds);
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
  PopulateIdsForNoteEvents(noteEvents);

  auto runs = FindNoteRunsNoNoteOffEvents(noteEvents, MIN_DIFF, MAX_DIFF);

  const std::vector<int> expectedIds = { 1, 2, 3, 4 };
  REQUIRE(runs.size() == 1);
  REQUIRE(runs.front().m_ids == expectedIds);
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
  PopulateIdsForNoteEvents(noteEvents);

  auto runs = FindNoteRunsNoNoteOffEvents(noteEvents, MIN_DIFF, MAX_DIFF);

  const std::vector<int> expectedIds1 = { 1, 2, 3 };
  const std::vector<int> expectedIds2 = { 5, 6, 7 };

  REQUIRE(runs.size() == 2);
  REQUIRE(runs[0].m_ids == expectedIds1);
  REQUIRE(runs[0].m_isUp == false);
  REQUIRE(runs[1].m_ids == expectedIds2);
  REQUIRE(runs[1].m_isUp == true);
}

TEST_CASE("2 note runs, note-on and note-off events", "[NoteRun]")
{
  // The events should be in time order.
  NoteEvents noteEvents
  {
    // pitch, time, type, coord
    NoteEvent(60, 0.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(60, 1.f,  NoteEventType::NOTE_OFF, {0, 0}),
    NoteEvent(61, 2.f,  NoteEventType::NOTE_ON,  {0, 0}), // run start
    NoteEvent(61, 3.f,  NoteEventType::NOTE_OFF, {0, 0}), 
    NoteEvent(60, 4.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(60, 5.f,  NoteEventType::NOTE_OFF, {0, 0}),
    NoteEvent(59, 6.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(59, 7.f,  NoteEventType::NOTE_OFF, {0, 0}),
    NoteEvent(63, 8.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(63, 9.f,  NoteEventType::NOTE_OFF, {0, 0}),
    NoteEvent(70, 10.f,  NoteEventType::NOTE_ON,  {0, 0}), // run start
    NoteEvent(70, 11.f,  NoteEventType::NOTE_OFF, {0, 0}), 
    NoteEvent(72, 12.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(72, 13.f,  NoteEventType::NOTE_OFF, {0, 0}),
    NoteEvent(73, 14.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(73, 15.f,  NoteEventType::NOTE_OFF, {0, 0}),
    NoteEvent(60, 16.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(60, 17.f,  NoteEventType::NOTE_OFF, {0, 0}),
  };
  PopulateIdsForNoteEvents(noteEvents);

  auto runs = FindNoteRuns(noteEvents, MIN_DIFF, MAX_DIFF);

  const std::vector<int> expectedIds1 = { 2, 4, 6 };
  const std::vector<int> expectedIds2 = { 10, 12, 14 };

  REQUIRE(runs.size() == 2);
  REQUIRE(runs[0].m_ids == expectedIds1);
  REQUIRE(runs[0].m_isUp == false);
  REQUIRE(runs[1].m_ids == expectedIds2);
  REQUIRE(runs[1].m_isUp == true);
}

