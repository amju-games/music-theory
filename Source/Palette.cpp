// * Amjula music theory * (c) Copyright 2024 Juliet Colman

#include <algorithm>
#include <cmath>
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

const Colour& Palette::GetColour(float f) const
{
  int key = std::round(f * static_cast<float>(m_colours.size() - 1));
  key = std::clamp(key, 0, static_cast<int>(m_colours.size() - 1));
  return m_colours[key];
}

}
