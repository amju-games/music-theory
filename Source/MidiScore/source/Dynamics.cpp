#include <algorithm>
#include <array>
#include <iostream>
#include "Dynamics.h"

namespace MidiScore
{
static std::string s_lastString;

void Dynamics::SetLastDynamicsString(const std::string& s)
{
  s_lastString = s;
}

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

std::string Dynamics::ToString() const
{
  if (m_string == s_lastString)
  {
    // Don't repeat the same dynamic marking
    return "";
  }

  if (m_string != "")
  {
    // Remember last marking, but not if it's empty
    s_lastString = m_string;
  }

  return m_string;
}

std::string Dynamics::SetMark()
{
  auto s = VelocityToString(m_velocity);
  m_string += s;
  return s;
}
}

