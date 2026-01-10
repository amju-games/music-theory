#pragma once

#include <vector>

namespace Amju
{
  // Note on/off events, which we load along with glyphs. This makes it easier to
  //  add midi note events to a score.
  struct NoteEvent
  {
    NoteEvent() = default;
    NoteEvent(int note, float time, bool onNotOff) :
      m_note(note), m_time(time), m_onNotOff(onNotOff) {}

    bool operator<(const NoteEvent& ne) const
    {   
      return m_time < ne.m_time;
    }   

    // Midi note value
    int m_note = 0;
    // Times are 0..1 animation values, not time in seconds
    float m_time = 0;
    // MIDI note on or note off event
    bool m_onNotOff = true;
  };  

  // Vector of note events, sorted by time.
  // NB For polyphony, consecutive events could have the same time.
  using NoteEvents = std::vector<NoteEvent>;
}

