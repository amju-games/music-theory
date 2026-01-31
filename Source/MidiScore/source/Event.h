// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#pragma once

#include <string>
#include <vector>

namespace MidiScore
{
enum class TimeSig;

enum class TimeVal
{
  SEMIQUAVER,
  QUAVER,
  CROTCHET, 
  MINIM,
  SEMIBREVE,
  SB2,
  SB4
};

std::string TimeValString(TimeVal t);

enum class EventType
{
  NOTE, REST, BARLINE, TIE, CHORD_START, CHORD_END, TIME_SET
};

struct Event
{
  int m_start = 0; // in tpq ticks
  int m_duration = 0; // in tpq ticks
  int m_end = 0; // in tpq ticks
  TimeVal m_timeVal = TimeVal::CROTCHET;
  int m_dots = 0; // multiply time val by 1.5 per dot
  int m_pitch = 0; // MIDI pitch
  int m_velocity = 64; 

  // For time set event, the output time, given in number of crotchets
  //  from the start of the piece. 
  // (We don't expose midi-specific tpq value)
  float m_timeSetVal = 0; 
  
  EventType m_type = EventType::NOTE;

  std::string ToString() const;
  std::string NoteToStringNoDuration() const;
  std::string DurationString() const;

  // Set timeval enum and dots in this Event, given duration and tpq. 
  void SetTimeVal(int tpq);

  bool IsNote() const { return m_type == EventType::NOTE; }
  bool IsRest() const { return m_type == EventType::REST; }
  bool IsBarLine() const { return m_type == EventType::BARLINE; }
  bool IsTie() const { return m_type == EventType::TIE; }
  bool IsChordStart() const { return m_type == EventType::CHORD_START; }
  bool IsChordEnd() const { return m_type == EventType::CHORD_END; }
};

using Events = std::vector<Event>;

// Used when we read note events from MIDI input.
void AppendNoteEventToEvents(const Event& e, Events& events);

// Insert rests in the given events vec, where there are gaps between
//  the end and start time of two consecutive events.
void InsertRests(int tpq, Events& events);

void InsertBarLines(int tpq, TimeSig ts, Events& events);

void InsertChordMarkers(Events& events);

void InsertTimeSetEvents(int tpq, Events& events);
}

