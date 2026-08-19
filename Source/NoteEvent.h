#pragma once

#include <string>
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

  // Convenience function for debug messages
  std::string ToString(NoteEventType net);

  // ** NoteEvent **
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
    bool IsNoteEvent() const { return IsNoteOnEvent() || IsNoteOffEvent(); }
 
    bool IsRestOnEvent() const { return m_type == NoteEventType::REST_ON; }
    bool IsRestOffEvent() const { return m_type == NoteEventType::REST_OFF; }
    bool IsRestEvent() const { return IsRestOnEvent() || IsRestOffEvent(); }

    const Vec2f& GetPos() const { return m_pos; }

    // Event ID. The IDs are the same as the indices in a NoteEvents 
    //  vector, but if we are not iterating over the vector, we don't 
    //  know the index, so we need this redundancy.
    int GetId() const { return m_id; }

    // Convenience for debugging
    std::string ToString() const;

    // Midi note value, i.e. pitch (if applicable - not for rests)
    int m_note = -1; // not set 

    // Times are 0..1 animation values, not time in seconds
    float m_time = 0;
    // Type of event
    NoteEventType m_type;
    // Midi note volume, 0..127
    int m_volume = 127;
    // local (x, y) position on the score
    Vec2f m_pos; 

    // ID. Set after the sort by time and so give position in the song.
    int m_id = -1; // not set yet
  };  

  // Vector of note events, sorted by time.
  // NB For polyphony, consecutive events could have the same time.
  using NoteEvents = std::vector<NoteEvent>;

  // Find the note on event for the given note off event.
  // Linear search backwards for a note on event with the same pitch
  //  as the given event.
  // Returns id of corresponding note on event if found, or one of the
  //  error codes below, which are < 0.
  int FindNoteOnEventForNoteOffEvent(
    const NoteEvents& events, int noteOffEventId);

  // Error codes for above function return value
  static const int NOTE_ON_EVENT_NOT_FOUND = -1;
  static const int NOT_A_NOTE_OFF_EVENT = -2;
}

