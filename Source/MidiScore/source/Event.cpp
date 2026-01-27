// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include "Event.h"

namespace MidiScore
{
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
      gapEvent.m_isRest = true;
      gapEvent.SetTimeVal(tpq);

      it = events.insert(it, gapEvent);
      ++it;
    }
    t = it->m_end;
  }
}
}

