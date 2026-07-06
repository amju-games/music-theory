// AUTO-GENERATED
// * Amjula music theory *
// (c) Copyright 2026 Juliet Colman

#pragma once

#include <tuple>

namespace Amju
{
static const char* const VERSION_STRING = "0.3.0 (1)";
static const int MAJOR = 0;
static const int MINOR = 3;
static const int PATCH = 0;
static const int BUILD = 1;

// Version comparison helper
inline bool IsVersionAtLeast(int major, int minor, int patch) 
{
  return std::tie(MAJOR, MINOR, PATCH) >= std::tie(major, minor, patch);
}
}

