#include <algorithm>
#include <array>
#include "Dynamics.h"

namespace MidiScore
{
std::string VelocityToString(int velocity)
{
  // Convert velocity 0-127 into index 0-7.
  int i = std::clamp(velocity / 16, 0, 7);
  assert(i >= 0);
  assert(i < 8);
  static const std::array<std::string, 8> STRS =
  {{
    "ppp", "pp", "p", "mp", "mf", "f", "ff", "fff"
  }}; 
  return STRS[i];
}

void Dynamics::SetFromPrevEvent(const Dynamics& prevDynamics)
{
  m_velChange = m_velocity - prevDynamics.m_velocity;
}

void Dynamics::SetString()
{
  m_string = VelocityToString(m_velocity);
}
}

