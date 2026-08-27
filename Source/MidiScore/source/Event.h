// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#pragma once

#include <string>
#include <vector>
#include "Dynamics.h"

namespace MidiScore
{
enum class Clef;
enum class KeySig;
enum class TimeSig;

enum class TimeVal
{
  NONE,

  QQQ,
  SEMIQUAVER,
  QUAVER,
  CROTCHET, 
  MINIM,
  SEMIBREVE,
  SB2,
  SB4
};

// * TimeValString *
// Return string for timeval, in juliet notation format, suitable for
//  makescore.
std::string TimeValString(TimeVal t, int dots = 0);

// Return TimeVal from string, for converting command-line params to 
//  TimeVals.
TimeVal GetTimeValFromString(const std::string& s);

// * CalcTpqMultipleForTimeVal *
// Calc tpq multiplied according to the TimeVal. A crotchet time val means
//  a multiplier of 1; minim multiplies by 2, quaver multiplies by 0.5, etc.
// All the arithmetic uses ints, and event start times and durations are ints,
//  so there are no float precision issues to worry about.
// 
// E.g.: CalcTpqMultipleForTimeVal(16, <minim>) => 32
int CalcTpqMultipleForTimeVal(int tpq, TimeVal t);

enum class EventType
{
  NOTE, REST, BARLINE, TIE, CHORD_START, CHORD_END, TIME_SET,
  CLEF,
  KEY_SIG,
  TIME_SIG
};

struct Event
{
  int m_start = 0; // in tpq ticks
  int m_unquantisedStart = -1; 
  // ..in tpq ticks. Initially -1 so we can check it has been set.

  int m_duration = 0; // in tpq ticks
  int m_unquantisedDuration = -1; // in tpq ticks. 
  int m_end = 0; // in tpq ticks; start + duration
  TimeVal m_timeVal = TimeVal::NONE;
  int m_dots = 0; // multiply time val by 1.5 per dot
  int m_pitch = 0; // MIDI pitch
  bool m_isWholeBar = false; // for rests

  Dynamics m_dynamics;

  // For time set event, the output time, given in number of crotchets
  //  from the start of the piece. 
  // (We don't expose midi-specific tpq value)
  float m_timeSetVal = 0; 
  
  EventType m_type = EventType::NOTE;

  Clef m_clef; // for clef events; initialised in ctor
  TimeSig m_timeSig; // for time sig events
  KeySig m_keySig; // for key sig events, and for note events, the prevailing key sig, used to output pitch correctly.

  Event();
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
  bool IsClef() const { return m_type == EventType::CLEF; }
  bool IsKeySig() const { return m_type == EventType::KEY_SIG; }
  bool IsTimeSig() const { return m_type == EventType::TIME_SIG; }
};

using Events = std::vector<Event>;

// Used when we read note events from MIDI input.
// NB Event is passed by value, as we will update dynamics members.
// Also, we might need to split a note event into multiple tied
//  notes. E.g.  4/4 <60> m t c t q <61>  q |
// Pass in time sig so we split based on bars (although bar line
//  events are not added yet.) Also pass in anacrusis length, so we
//  split on bar lines correctly when there is an anacrusis.
// Default is to NOT split notes on beats, the algo for doing it is wrong.
void AppendNoteEventToEvents(
  int tpq, Event e, Events& events, TimeSig ts, bool yesSplitOnBeat = false,
  int anacrusisTicks = 0);

// Call first, when the only events are notes, for simplicity.
void InsertDynamics(Events& events);

// Insert rests in the given events vec, where there are gaps between
//  the end and start time of two consecutive events.
// NB we may split rests to make up the time value, as for
//  AppendNoteEventToEvents above. e.g. <m.> r <q> r
// Time sig is required so we know if dotted rests are allowed.
void InsertRests(int tpq, Events& events, TimeSig ts);

// Insert bar lines into the given events vec.
// We have already split long notes at bar line positions.
void InsertBarLines(int tpq, TimeSig ts, Events& events, int numBars, int anacrusisTicks);

void InsertChordMarkers(Events& events);

void InsertTimeSetEvents(int tpq, Events& events);

// Reverse some events - we expect them to be notes and rests.
// This reverses their order and recalculates start and end times.
void Reverse(Events& events);

// Helper function, exposed for testing.
// Calc the end time of the bar; equivalently, the start time of the next bar.
// pos: time position within the bar
// ts: the time sig
// anacrusisTicks: length of incomplete, introductory bar, at the scale
//  of tpqs. E.g. anacrusis of a minim is 2 * tpq ticks. 
int CalcEndOfBar(int tpq, int pos, TimeSig ts, int anacrusisTicks = 0);
}

