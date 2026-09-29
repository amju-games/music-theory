#include "AutoPlayer.h"
#include "catch.hpp"

using namespace Amju;

TEST_CASE("Check one note on/off event works", "[AutoPlayer]")
{
  AutoPlayer ap;

  // Midi pitch, time, type (i.e. note or rest on or off), pos
  NoteEvents score = 
  { 
    { 60, 2.f, NoteEventType::NOTE_ON, {} },
    { 60, 3.f, NoteEventType::NOTE_OFF, {} },
    { 61, 5.f, NoteEventType::NOTE_ON, {} },
    { 61, 7.f, NoteEventType::NOTE_OFF, {} },
  }; 

  // Generate auto-player music kb messages for score, with default skill.
  auto messages = ap.GenerateMessages(score, {});

  int numMessages = messages.size();
  REQUIRE(numMessages == score.size());

  for (int i = 0; i < numMessages; ++i)
  {
    // Check notes (pitches) match
    REQUIRE(messages[i]->m_event.m_note == score[i].m_note);
    // Check execute time of message matches note event time
    REQUIRE(messages[i]->m_time == score[i].m_time);

    // TODO Check velocity/note on/off types match
  }
}

// TODO A.p. can also do bum notes during rests, etc.
