// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>

bool Contains(const std::string& s, char c);
std::string Remove(std::string& s, char c);

// Like to_string, but chops trailing zeroes from floats
template <typename T> std::string Str(T f);

std::string GetStr(std::string s);

std::string LineEnd(bool oneLine);

