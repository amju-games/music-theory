#include "catch.hpp"
#include "NoteEvent.h"
#include "Utils.h"

using namespace Amju;

TEST_CASE("Find note-on for note-off events", "[NoteEvent]")
{
  // The events should be in time order.
  NoteEvents noteEvents
  {
    // pitch, time, type, coord
    NoteEvent(60, 0.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(60, 1.f,  NoteEventType::NOTE_OFF, {0, 0}),
    NoteEvent(61, 2.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(60, 3.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(61, 4.f,  NoteEventType::NOTE_OFF, {0, 0}), 
    NoteEvent(60, 5.f,  NoteEventType::NOTE_OFF, {0, 0}),
    //NoteEvent(59, 6.f,  NoteEventType::NOTE_ON,  {0, 0}), // no note on event
    NoteEvent(59, 6.f,  NoteEventType::NOTE_OFF, {0, 0}),
    NoteEvent(60, 7.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(60, 8.f,  NoteEventType::NOTE_OFF, {0, 0}),


    NoteEvent(70, 10.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(70, 11.f,  NoteEventType::NOTE_OFF, {0, 0}), 
    NoteEvent(72, 12.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(72, 13.f,  NoteEventType::NOTE_OFF, {0, 0}),
    NoteEvent(73, 14.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(73, 15.f,  NoteEventType::NOTE_OFF, {0, 0}),
    NoteEvent(60, 16.f,  NoteEventType::NOTE_ON,  {0, 0}),
    NoteEvent(60, 17.f,  NoteEventType::NOTE_OFF, {0, 0}),
  };  
  PopulateIdsForNoteEvents(noteEvents);

  // int FindNoteOnEventForNoteOffEvent(
  //    const NoteEvents& events, int noteOffEventId);

  REQUIRE(FindNoteOnEventForNoteOffEvent(noteEvents, 1) == 0); // pitch 60
  REQUIRE(FindNoteOnEventForNoteOffEvent(noteEvents, 4) == 2); // pitch 61

  // pitch 59, no note on event
  REQUIRE(FindNoteOnEventForNoteOffEvent(noteEvents, 6) == NOTE_ON_EVENT_NOT_FOUND); 

  // pitch 60, not a note off event
  REQUIRE(FindNoteOnEventForNoteOffEvent(noteEvents, 7) == NOT_A_NOTE_OFF_EVENT); 
}

