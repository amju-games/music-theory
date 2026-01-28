// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include "Event.h"

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

void FillGapsWithRests(int tpq, Events& events)
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
}

