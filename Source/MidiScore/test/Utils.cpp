#include "Utils.h"

namespace MidiScore
{
// Create one note event
Event n(int pitch, int start, int duration, int tpq)
{
  Event e;
  e.m_type = EventType::NOTE;
  e.m_pitch = pitch;
  e.m_unquantisedStart = start;
  e.m_start = start;
  e.m_unquantisedDuration = duration;
  e.m_duration = duration;
  e.m_end = start + duration;
  e.SetTimeVal(tpq);
  return e;
}

// Create a rest event
Event r(int start, int duration, int tpq)
{
  Event e;
  e.m_type = EventType::REST;
  e.m_start = start;
  e.m_duration = duration;
  e.m_end = start + duration;
  e.SetTimeVal(tpq);
  return e;
}

// Create a barline event
Event barline(int start, int tpq) 
{
  Event e;
  e.m_type = EventType::BARLINE;
  e.m_start = start;
  e.m_duration = 0; 
  e.m_end = start + e.m_duration;
  e.SetTimeVal(tpq);
  return e;
}
}

