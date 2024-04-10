// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <algorithm> // remove
#include "Utils.h"

float Interp(float f0, float f1, float t)
{
  return f0 + (f1 - f0) * t;
}

bool IsBeam(const std::string& s)
{
  return s == "-" || s == "=" || s == "--" || s == "==" | s == "-=" || s == "=-";
}

bool IsRest(const std::string& s)
{
  return Contains(s, 'r');
}

void Trim(std::string& s)
{
  while (!s.empty() && (s[0] == ' ' || s[0] == '\r' || s[0] == '\n'))
  {
    s = s.substr(1);
  }
  while (!s.empty() && (s[s.size() - 1] == ' ' || s[s.size() - 1] == '\r' || s[s.size() - 1] == '\n'))
  {
    s = s.substr(0, s.size() - 1);
  }
}

bool Contains(const std::string& s, char c)
{
  return std::find(s.begin(), s.end(), c) != s.end();
}

bool Contains(const std::string& containing, const std::string& substr)
{
  return containing.find(substr) != std::string::npos;
}

std::string Remove(std::string& s, char c)
{
  s.erase(std::remove(s.begin(), s.end(), c), s.end());
  return s;
}

bool IsQuote(char c)
{
  return c == '"';
}

// Strip " char from beginning and end of string
void StripQuotes(std::string& s)
{
  if (s.size() < 2)
  {
    return;
  }
  if (IsQuote(s[0]))
  {
    s = s.substr(1, s.size() - 2);
  }
}

