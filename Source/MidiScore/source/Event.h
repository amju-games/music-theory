#pragma once

#include <string>
#include <vector>

namespace MidiScore
{
enum class TimeVal
{
  SEMIQUAVER,
  QUAVER,
  CROTCHET, 
  MINIM,
  SEMIBREVE
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
  bool m_isRest = false; // arghh

  std::string ToString() const;
};

using Events = std::vector<Event>;
}

