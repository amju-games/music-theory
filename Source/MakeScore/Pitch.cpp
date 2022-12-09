// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include "Pitch.h"

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
  return i > 0 && i < 128;
}

int GetPitch(const std::string& s)
{
  int i = atoi(s.c_str());
  if (i > 0 && i < 128)
  {
    return i;
  }
  return atoi(s.substr(1, s.size() - 2).c_str());
}

