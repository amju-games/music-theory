#include "catch.hpp"
#include "NoteEvent.h"
#include "Utils.h"

using namespace Amju;

// Helper: make note event vec and populate IDs
static NoteEvents MakeNoteEvents()
{
  // The events should be in time order.
  NoteEvents noteEvents
  {
    // pitch, time, type, coord
    NoteEvent(60, 0.00f,  NoteEventType::NOTE_ON,  {0, 0}), // 0
    NoteEvent(60, 0.01f,  NoteEventType::NOTE_OFF, {0, 0}), // 1
    NoteEvent(61, 0.02f,  NoteEventType::NOTE_ON,  {0, 0}), // 2
    NoteEvent(60, 0.03f,  NoteEventType::NOTE_ON,  {0, 0}), // 3
    NoteEvent(61, 0.04f,  NoteEventType::NOTE_OFF, {0, 0}), // 4
    NoteEvent(60, 0.05f,  NoteEventType::NOTE_OFF, {0, 0}), // 5
    //NoteEvent(59, 0.06f,  NoteEventType::NOTE_ON,  {0, 0}), // no note on event
    NoteEvent(59, 0.06f,  NoteEventType::NOTE_OFF, {0, 0}), // 6
    NoteEvent(60, 0.07,  NoteEventType::NOTE_ON,  {0, 0}), // 7
    NoteEvent(60, 0.08,  NoteEventType::NOTE_OFF, {0, 0}), // 8
    NoteEvent(70, 0.10f,  NoteEventType::NOTE_ON,  {0, 0}), // 19
    NoteEvent(70, 0.11f,  NoteEventType::NOTE_OFF, {0, 0}), // 11
    NoteEvent(72, 0.12f,  NoteEventType::NOTE_ON,  {0, 0}), // 12
    NoteEvent(72, 0.13f,  NoteEventType::NOTE_OFF, {0, 0}), // 13
    NoteEvent(73, 0.14f,  NoteEventType::NOTE_ON,  {0, 0}), // 14
    NoteEvent(73, 0.15f,  NoteEventType::NOTE_OFF, {0, 0}), // 15
    NoteEvent(60, 0.16f,  NoteEventType::NOTE_ON,  {0, 0}), // 16
    NoteEvent(60, 0.17f,  NoteEventType::NOTE_OFF, {0, 0}), // 17
  };  
  PopulateIdsForNoteEvents(noteEvents);
  return noteEvents;
}

TEST_CASE("Find note-on for note-off events", "[NoteEvent]")
{
  // Note events in time order.
  const NoteEvents noteEvents = MakeNoteEvents();

  REQUIRE(FindNoteOnEventForNoteOffEvent(noteEvents, 1) == 0); // pitch 60
  REQUIRE(FindNoteOnEventForNoteOffEvent(noteEvents, 4) == 2); // pitch 61

  // pitch 59, no note on event
  REQUIRE(FindNoteOnEventForNoteOffEvent(noteEvents, 6) == NOTE_ON_EVENT_NOT_FOUND); 

  // pitch 60, not a note off event
  REQUIRE(FindNoteOnEventForNoteOffEvent(noteEvents, 7) == NOT_A_NOTE_OFF_EVENT); 
}

TEST_CASE("Find ID for time", "[NoteEvent]")
{
  // Note events in time order.
  const NoteEvents noteEvents = MakeNoteEvents();
 
  REQUIRE(FindNoteEventForTime(noteEvents, 0.00f) == 1); // boundary cond.
  REQUIRE(FindNoteEventForTime(noteEvents, 0.005f) == 1); // 0th element found

  // 0th element found, not 1th
  REQUIRE(FindNoteEventForTime(noteEvents, 0.01f) == 2); 

  REQUIRE(FindNoteEventForTime(noteEvents, 0.135f) == 13);  

  // Finds final event
  REQUIRE(FindNoteEventForTime(noteEvents, 0.17f) == 16);  
  REQUIRE(FindNoteEventForTime(noteEvents, 0.999f) == 16);  
}

