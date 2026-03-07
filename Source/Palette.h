// * Amjula music theory * (c) Copyright 2024 Juliet Colman

#pragma once

#include <vector>
#include <Colour.h>
#include <RCPtr.h>

namespace Amju
{
class File;

// * Palette *
// Colours for notes on score and keyboard.
// Read texture from file, then use GetColour to look up MIDI
//   note number -> colour.
// Number of colours in texture should be 12*n, right?! 
class Palette : public RefCounted
{
public:
  // Look up colour, modulus used to keep key in range.
  const Colour& GetColour(int key) const; 

  bool Load(const std::string& imageFilename);

private:
  using ColourVec = std::vector<Colour>;
  ColourVec m_colours;
};
}
