#include "AutoPlayer.h"

namespace Amju
{
MusicKbMsg* GenerateOneMessage(const NoteEvent& ne)
{
  const int pitch = ne.m_note; //GetPitch();
  //const int velocity = ne.m_volume; //GetVelocity();
  const bool isNoteOn = ne.IsNoteOnEvent();

  auto msg = new MusicKbMsg(MusicKbEvent(pitch, isNoteOn));
  // Set time of message: this is a perfect match, but we could
  //  add some randomness (within limits so the message still corresponds
  //  to its original scored note event).
  msg->m_time = ne.m_time;
  return msg;
}

MusicMessages AutoPlayer::GenerateMessages(
  const NoteEvents& song, const Skill& skill)
{
  MusicMessages res;

  // For each event in the scored song, decide whether to generate
  //  a music message, and if so, how accurate it is.
  for (const auto& ne : song)
  {
    switch (ne.m_type)
    {
    case NoteEventType::NOTE_ON:
      // Generate a note on message, unless we decide to skip the note
      //  entirely. Time range????
      res.emplace_back(GenerateOneMessage(ne));
      break;

    case NoteEventType::NOTE_OFF:
      // Generate a note off message, if we generated a note on message
      //  previously. 
      res.emplace_back(GenerateOneMessage(ne));
      break;

    case NoteEventType::REST_ON:
    case NoteEventType::REST_OFF:
      break;
    }
  }

  return res;
}
}

