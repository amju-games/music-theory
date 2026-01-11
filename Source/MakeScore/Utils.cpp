// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <algorithm> // remove
#include "Utils.h"

std::string LineEnd(bool oneLine)
{
  // ";" as delimiter is deprecated; we can string all the lines together
  //  as a simple postprocessing step if we need to.
  return ("\n");
}

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

Strings Split(const std::string& cs, char c)
{
  Strings r;
  std::string s(cs);
  while (true) 
  {
    size_t i = s.find(c);
    if (i == std::string::npos)
    {   
      // No special character found, so push back the entire string and finish.
      r.push_back(s);
#ifdef SPLIT_DEBUG
      std::cout << "Split: final string: " << s.c_str() << "\n";
#endif
      break; 
    }         
    else
    {   
      // Found the special character. 
      // Push back whatever is before the character, then work on the remainder
      // of the string.
#ifdef SPLIT_DEBUG
      std::cout << "Split: string: " << s.substr(0, i).c_str() << " rem: " << s.substr(i + 1).c_str() << "\n";
#endif 
      r.push_back(s.substr(0, i));
      s = s.substr(i + 1); 
      Trim(s); // Allow whitespace, get rid of it here
    }   
  }
  return r;
}
