// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <string>

namespace Amju
{
class File;

// * Version *
// We want to save the oldest version on to player device, so we
//  know when they started playing.
// We might also want to tell players that they should update.
// We should display the version number on screen so players can report
//  the version in bug reports.

struct ThisVersion
{
  static const int MAJOR = 0;
  static const int MINOR = 1;
  static const int PATCH = 0; 
};

// Serialisable object we can compare.
struct Version
{
  Version() = default;
  std::string ToString() const;
  bool operator<(const Version&) const;
  bool Load(File*);
  bool Save(File*);

  int m_major = ThisVersion::MAJOR;
  int m_minor = ThisVersion::MINOR;
  int m_patch = ThisVersion::PATCH;
};

Version& GetVersion();
}

