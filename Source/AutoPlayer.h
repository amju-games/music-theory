#pragma once

#include "MusicEvent.h"
#include "NoteEvent.h"

namespace Amju
{
// * Auto Player *
// Sends player input events with configurable skill.
class AutoPlayer
{
public:
  struct Skill
  {
    // Correct note hit rate, from 0 (never correct) ..1 (perfect)
    float m_correctNoteP = 1.f;

    // TODO Timing accuracy
  };

  // Generate note on and note off messages for the given song,
  //  with the given skill level.
  // We then enqueue each of these messages in the message queue.
  MusicMessages GenerateMessages(const NoteEvents& song, const Skill&);
};
}

