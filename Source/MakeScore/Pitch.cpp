// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <array>
#include "Pitch.h"

std::string Pitch::ToString() const
{
  std::string res;
  if (!m_step.empty())
  {
    res += m_step + std::to_string(m_octave);
    if (m_alter > 0) 
    {
      res += std::string(m_alter, '+');
    }
    else if (m_alter < 0)
    {
      res += std::string(-m_alter, '-');
    }
    res += " ";
  }
  res += "midi: " + std::to_string(m_midi);
  return res;
}

void Pitch::CalcMidi()
{
  // Base midi pitch values for the seven notes (steps)
  constexpr std::array<int, 7> PITCHES = 
  {
    9 /* a */, 11 /* b */, 0 /* c */, 2 /* d */, 4 /* e */, 5 /* f */, 7 /* g */
  };  
  char step = m_step[0];
  if (step >= 'a' && step <= 'g') 
  {
    step -= 'a';
  }
  else if (step >= 'A' && step <= 'G') 
  {
    step -= 'A';
  }
  assert(step >= 0); 
  assert(step < 7); 
  m_midi = PITCHES[step];
  m_midi += m_alter;
  m_midi += 12 * (m_octave + 1); 
}

bool IsDeferredPitch(const std::string& s)
{
  return s.size() > 2 && s[0] == '<' && s.back() == '>' &&
    IsImmediatePitch(s.substr(1, s.size() - 2));
}

// Return true if token defines a pitch, and should generate a note
//  immediately
bool IsImmediatePitch(const std::string& s)
{
  // Pitch is defined by MIDI note value; so a pitch token is an int
  //  between 1-127.
  int i = atoi(s.c_str());
  if (i > 0 && i < 128)
  {
    return true;
  }
  if (s.size() >= 2)
  {
    return (s[0] >= 'a' && s[0] <= 'g' && s[1] >= '0' && s[1] <= '9');
  }
  return false;
}

Pitch GetPitch(const std::string& cs)
{
  std::string s = cs;
  if (s[0] == '<') // deferred -- chop off <>
  {
    s = s.substr(1, s.size() - 2);
  }

  Pitch pitch;
  int i = atoi(s.c_str());
  if (i > 0 && i < 128)
  {
    pitch.m_midi = i;
    return pitch;
  }
  pitch.m_step = s.substr(0, 1);
  while (s.back() == '+')
  {
    s.pop_back();
    pitch.m_alter++;
  }
  while (s.back() == '-')
  {
    s.pop_back();
    pitch.m_alter--;
  }
  pitch.m_octave = atoi(s.substr(1).c_str());
  pitch.CalcMidi();
  return pitch;
}

