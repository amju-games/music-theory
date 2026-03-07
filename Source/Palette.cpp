// * Amjula music theory * (c) Copyright 2024 Juliet Colman

#include <GuiDecColourPalette.h>
#include "Palette.h"

namespace Amju
{
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
}
