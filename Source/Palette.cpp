// * Amjula music theory * (c) Copyright 2024 Juliet Colman

#include <algorithm>
#include <cmath>
#include <GuiDecColourPalette.h>
#include "Palette.h"

namespace Amju
{
Resource* PaletteLoader(const std::string& resName)
{
  // ".pal" resources are images; strip off final extension and
  //  load the palette. 
  // E.g. spectrum.png.pal -> spectrum.png
  // (It has to be a png.)
  std::string imageFilename = GetFileNoExt(resName);
  auto palette = new Palette;
  if (!palette->Load(imageFilename)) 
  {
    Assert(0);
    return nullptr; 
  }
  return palette;
}

bool Palette::Load(const std::string& imageFilename)
{
  auto optPalette = ImageToColourVec(imageFilename);
  if (!optPalette) return false;
  m_colours = *optPalette;
  return true;
}

const Colour& Palette::GetColour(int key) const
{
  key = key % m_colours.size();
  return m_colours[key];
}

const Colour& Palette::GetColour(float f) const
{
  int key = static_cast<int>(
    std::round(f * static_cast<float>(m_colours.size() - 1)));
  key = std::clamp(key, 0, static_cast<int>(m_colours.size() - 1));
  return m_colours[key];
}

}
