// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#pragma once

#include <string>
#include <vector>
#include "Dynamics.h"

namespace MidiScore
{
enum class TimeSig;

enum class TimeVal
{
  QQQ,
  SEMIQUAVER,
  QUAVER,
  CROTCHET, 
  MINIM,
  SEMIBREVE,
  SB2,
  SB4
};

std::string TimeValString(TimeVal t, int dots = 0);

TimeVal GetTimeValFromString(const std::string& s);

enum class EventType
{
  NOTE, REST, BARLINE, TIE, CHORD_START, CHORD_END, TIME_SET
};

struct Event
{
  int m_start = 0; // in tpq ticks
  int m_unquantisedStart = -1; // in tpq ticks. -1 so we can check it's set
  int m_duration = 0; // in tpq ticks
  int m_unquantisedDuration = -1; // in tpq ticks. 
  int m_end = 0; // in tpq ticks; start + duration
  TimeVal m_timeVal = TimeVal::CROTCHET;
  int m_dots = 0; // multiply time val by 1.5 per dot
  int m_pitch = 0; // MIDI pitch
  bool m_isWholeBar = false; // for rests

  Dynamics m_dynamics;

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

  // Quantise start time, to the given resolution, which defaults
  //  to the finest grain.
  void QuantiseStartTime(int tpq, TimeVal resolution = TimeVal::QQQ);

  void QuantiseDuration(int tpq, TimeVal resolution = TimeVal::QQQ);
  
  bool IsNote() const { return m_type == EventType::NOTE; }
  bool IsRest() const { return m_type == EventType::REST; }
  bool IsBarLine() const { return m_type == EventType::BARLINE; }
  bool IsTie() const { return m_type == EventType::TIE; }
  bool IsChordStart() const { return m_type == EventType::CHORD_START; }
  bool IsChordEnd() const { return m_type == EventType::CHORD_END; }
};

using Events = std::vector<Event>;

// Used when we read note events from MIDI input.
// NB Event is passed by value, as we will update dynamics members.
// Also, we might need to split a note event into multiple tied
//  notes. E.g.  4/4 <60> m t c t q <61>  q |
void AppendNoteEventToEvents(int tpq, Event e, Events& events);

// Call first, when the only events are notes, for simplicity.
void InsertDynamics(Events& events);

// Insert rests in the given events vec, where there are gaps between
//  the end and start time of two consecutive events.
// NB we may split rests to make up the time value, as for
//  AppendNoteEventToEvents above. e.g. <m.> r <q> r
void InsertRests(int tpq, Events& events);

void InsertBarLines(int tpq, TimeSig ts, Events& events);

void InsertChordMarkers(Events& events);

void InsertTimeSetEvents(int tpq, Events& events);
}

