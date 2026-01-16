#pragma once

#include <vector>
#include <Vec2.h>

namespace Amju
{
  enum class NoteEventType
  {
    NOTE_ON,
    NOTE_OFF,
    REST_ON,
    REST_OFF,
    // TODO Barlines? Every beat?
  };

  // Music events loaded from Score meta data.
  // Note and rest on/off events, with normalized time and 
  //  (x, y) coord in the score.
  struct NoteEvent
  {
    NoteEvent() = default;
  
    NoteEvent(int note, float time, NoteEventType type, const Vec2f& pos) :
      m_note(note), m_time(time), m_type(type), m_pos(pos) {}

    NoteEvent(int note, float time, NoteEventType type, int volume, const Vec2f& pos) :
      m_note(note), m_time(time), m_type(type), m_volume(volume), m_pos(pos) {}

    bool operator<(const NoteEvent& ne) const
    {   
      return m_time < ne.m_time;
    }   

    bool IsNoteOnEvent() const { return m_type == NoteEventType::NOTE_ON; }
    bool IsNoteOffEvent() const { return m_type == NoteEventType::NOTE_OFF; }
    bool IsRestOnEvent() const { return m_type == NoteEventType::REST_ON; }
    bool IsRestOffEvent() const { return m_type == NoteEventType::REST_OFF; }

    const Vec2f& GetPos() const { return m_pos; }

    // Midi note value (if applicable)
    int m_note = 0;
    // Times are 0..1 animation values, not time in seconds
    float m_time = 0;
    // Type of event
    NoteEventType m_type;
    // Midi note volume, 0..127
    int m_volume = 127;
    // local (x, y) position on the score
    Vec2f m_pos; 
  };  

  // Vector of note events, sorted by time.
  // NB For polyphony, consecutive events could have the same time.
  using NoteEvents = std::vector<NoteEvent>;

}

