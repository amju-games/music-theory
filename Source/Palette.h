// * Amjula music theory * (c) Copyright 2024 Juliet Colman

#pragma once

#include <vector>
#include <Colour.h>
#include <RCPtr.h>

namespace Amju
{
class File;

// * Palette *
// Colour palette: initially intended for notes on score and keyboard,
//  but can be useful in other contexts, to avoid hardcoding a list
//  of colours in code (yuck!).

// For notes on score and keyboard:
// Read texture from file, then use GetColour to look up MIDI
//   note number -> colour.
// Number of colours in texture should be 12*n, right?! 

// For general use: 
// Use GetColour(float) to get the closest colour in the sequence.
// TODO We could interpolate between the two nearest colours if required.
//
// *** NB Don't duplicate behaviour of GuiDecColourPalette, which already
// ***  sets a colour based on anim value!
class Palette : public RefCounted
{
public:
  // Look up colour, modulus used to keep key in range.
  const Colour& GetColour(int key) const; 

  // Alternative lookup: zeroToOne is in range (0..1).
  const Colour& GetColour(float zeroToOne) const; 
  
  bool Load(const std::string& imageFilename);

private:
  using ColourVec = std::vector<Colour>;
  ColourVec m_colours;
};
}
