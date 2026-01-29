// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include <iostream>
#include "Event.h"
#include "TimeSig.h"

namespace MidiScore
{
std::string TimeValString(TimeVal t)
{
  if (t == TimeVal::SEMIQUAVER) return "qq";
  if (t == TimeVal::QUAVER) return "q";
  if (t == TimeVal::CROTCHET) return "c";
  if (t == TimeVal::MINIM) return "m";
  return "sb";
}

std::string Event::ToString() const
{
  switch (m_type)
  {
  case EventType::REST:
    return TimeValString(m_timeVal) + "r" + (m_dots > 0 ? std::string(m_dots, '.') : "");
  
  case EventType::NOTE: 
    return "<" + TimeValString(m_timeVal) + (m_dots > 0 ? std::string(m_dots, '.') : "") +"> " + std::to_string(m_pitch);

  case EventType::BARLINE:
     return "|";

  case EventType::TIE:
     return "t";

  default:
    break;
  }
  return "";
}

// Set timeval enum in an Event, given duration and tpq. 
void Event::SetTimeVal(int tpq)
{
  if (m_duration >= 4 * tpq) 
  {
    m_timeVal = TimeVal::SEMIBREVE;
    if (m_duration == 6 * tpq) m_dots = 1;
  }
  else if (m_duration >= 2 * tpq) 
  {
    m_timeVal = TimeVal::MINIM;
    if (m_duration == 3 * tpq) m_dots = 1;
  }
  else if (m_duration >=     tpq) 
  {
    m_timeVal = TimeVal::CROTCHET;
    if (m_duration == (3 * tpq / 2)) m_dots = 1;
  }
  else if (m_duration >= tpq / 2) 
  {
    m_timeVal = TimeVal::QUAVER;
    if (m_duration == (3 * tpq / 4)) m_dots = 1;
  }
  else 
  {
    m_timeVal = TimeVal::SEMIQUAVER;
    if (m_duration == (3 * tpq / 8)) m_dots = 1;
  }
}

void InsertRests(int tpq, Events& events)
{
  int t = 0;
  for (auto it = events.begin(); it != events.end(); ++it)
  {
    if (it->m_start >  t)
    {
      int gap = it->m_start - t;

      Event gapEvent;
      gapEvent.m_duration = gap;
      gapEvent.m_start = t;
      gapEvent.m_end = gap + t;
      gapEvent.m_type = EventType::REST;
      gapEvent.SetTimeVal(tpq);

      it = events.insert(it, gapEvent);
      ++it;
    }
    t = it->m_end;
  }
}

Event MakeBarLine(int startTicks)
{
  Event e;
  e.m_type = EventType::BARLINE;
  e.m_start = startTicks;
  e.m_duration = 0;
  e.m_end = startTicks;
  return e;
}

Event MakeTie(int startTicks)
{
  Event e;
  e.m_type = EventType::TIE;
  e.m_start = startTicks;
  e.m_duration = 0;
  e.m_end = startTicks;
  return e;
}

void SplitNote(int tpq, Events& events, Events::iterator& it, int barLineTicks)
{
  // One note -> first part of note, bar line, tie, final part of note.
  // If the note value is greater than a bar length, we can repeat the
  //  splitting process.
  // Before:
  //  it -> <note>
  // After:
  //        <first note>
  //        <barline>
  //        <tie>
  //  it -> <second note>

  Event& firstNote = *it;
  Event secondNote(*it);
  firstNote.m_end = barLineTicks;
  firstNote.m_duration = firstNote.m_end - firstNote.m_start;
  firstNote.SetTimeVal(tpq);
  secondNote.m_start = barLineTicks;
  secondNote.m_duration = secondNote.m_end - secondNote.m_start;
  secondNote.SetTimeVal(tpq);
  ++it;
  it = events.insert(it, MakeBarLine(barLineTicks));
  ++it;
  it = events.insert(it, MakeTie(barLineTicks));
  ++it;
  it = events.insert(it, secondNote);
}

void InsertBarLines(int tpq, TimeSig ts, Events& events)
{
  // We don't know where the bar lines should fall (although after the 
  //  last note is probable, i.e. the last event is unlikely to be a 
  //  rest??) - so let the user specify an anacrusis, otherwise just
  //  start adding bar lines every time sig worth of tpq.

  int ticksForOneBar = tpq * BeatsInBar(ts);
  int bar = 1; // don't add barline at start  
  for (auto it = events.begin(); it != events.end(); ++it)
  {
    if (it->IsBarLine() || it->IsTie()) continue;

    int barLineTicks = bar * ticksForOneBar;
    if (it->m_start < barLineTicks && it->m_end > barLineTicks)
    {
      SplitNote(tpq, events, it, barLineTicks);
      bar++;
    }
    else if (it->m_start >= barLineTicks)
    {
      // (if >, there's a gap here: fill with a rest now?)

      it = events.insert(it, MakeBarLine(barLineTicks));
      bar++;
    }
  }
  events.push_back(MakeBarLine(bar * ticksForOneBar));
}
}

