// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include "Event.h"

namespace MidiScore
{
// Set timeval enum in an Event, given duration and tpq. 
void Event::SetTimeVal(int tpq)
{
  if (m_duration >= 4 * tpq) m_timeVal = TimeVal::SEMIBREVE;
  else if (m_duration >= 2 * tpq) m_timeVal = TimeVal::MINIM;
  else if (m_duration >=     tpq) m_timeVal = TimeVal::CROTCHET;
  else if (m_duration >= tpq / 2) m_timeVal = TimeVal::QUAVER;
  else m_timeVal = TimeVal::SEMIQUAVER;
}

void FillGapsWithRests(int tpq, Events& events)
{
  int t = 0;
  for (auto it = events.begin(); it != events.end(); ++it)
  {
    if (it->m_start >  t)
    {
      int gap = it->m_duration - t;
      Event gapEvent;
      gapEvent.m_duration = gap;
      gapEvent.m_start = t;
      gapEvent.m_end = gap + t;
      gapEvent.m_isRest = true;
      it = events.insert(it, gapEvent);
      ++it;
    }
    t = it->m_end;
  }
}
}

