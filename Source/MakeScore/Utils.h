// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <sstream>
#include <string>

void Trim(std::string& s);
bool Contains(const std::string& s, char c);
std::string Remove(std::string& s, char c);

// Strip " char from beginning and end of string
void StripQuotes(std::string& s);

bool IsQuote(char c);
// Like to_string, but chops trailing zeroes from floats
template <typename T> std::string Str(T f);

std::string GetStr(std::string s);

std::string LineEnd(bool oneLine);

// Str - convert given argument to a string
// For floats, this results in no trailing zeros, nice
template <typename T>
std::string Str(T f)
{
  std::stringstream ss;
  ss << f;
  return ss.str();
}

bool IsBeam(const std::string& s);

bool IsRest(const std::string& s);

float Interp(float f0, float f1, float t);
