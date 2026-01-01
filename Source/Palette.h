// * Amjula music theory * (c) Copyright 2024 Juliet Colman

#pragma once

#include <unordered_map>
#include <vector>
#include <Colour.h>
#include <RCPtr.h>

namespace Amju
{
class File;

// * Palette *
// Serialisable container of named colours.
// File format is just lines like this (comment/version lines are allowed):
// <colourName> = <colourValue>
// ColourValue is RGB[A] hex string
// E.g. 
//   red=ff0000
//   translucentblue=0000ff80
class Palette : public RefCounted
{
public:
  using ColourVec = std::vector<Colour>;
  using ColourMap = std::unordered_map<std::string, Colour>;

  ColourVec GetColours() const;

  const ColourMap& GetColourMap() const { return m_colours; }

  std::optional<Colour> GetColour(const std::string& colourName) const;

  bool Load(File& f);
  bool Save(File& f);

private:
  // Parse one line of file in Load()
  bool ParseLine(const std::string& s);

private:
  ColourMap m_colours;
};
}
