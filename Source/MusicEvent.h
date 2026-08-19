// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <vector>
#include <MessageQueue.h>

namespace Amju
{
// Compare two MIDI note pitches. Return true if the player
//  note is correct wrt the note on the score.
// We can ignore octave to make this easier, or be strict
//  depending on difficulty.
bool IsPlayerPitchCorrect(int playerNote, int scoreNote);

// Music KB Event fired when we key up or down.
struct MusicKbEvent 
{
  MusicKbEvent() = default;
  MusicKbEvent(int note, bool on) : m_note(note), m_on(on) {}
  int m_note = 0;
  bool m_on = true;
};

// Function called by a Message sent from the Music Keyboard
void OnMusicKbEvent(const MusicKbEvent&);

// Message type
struct MusicKbMsg : public Message
{
  MusicKbMsg(const MusicKbEvent& e) : m_event(e) {}

  virtual void Execute() override
  {
    OnMusicKbEvent(m_event);
  }

  MusicKbEvent m_event;
};

using MusicMessages = std::vector<RCPtr<MusicKbMsg>>;
}

