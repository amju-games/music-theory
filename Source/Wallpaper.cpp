// * Amjula music theory * (c) Copyright 2024 Juliet Colman

#include <array>
#include <vector>
#include <GuiDecColour.h>
#include "Wallpaper.h"

namespace Amju
{
static std::array<Colour, 2> GetColoursForLevel(int level)
{
  // TODO TEMP TEST
  static const std::vector<std::array<Colour, 2>> COLOURS 
  {
    { Colour { 1, 0, 0, 1 }, Colour { 0, 1, 1, 1 } },
    { Colour { 1, 1, 0, 1 }, Colour { 0, 0, 0, 1 } },
  };
  int wrapLevel = level % COLOURS.size();
  return COLOURS[wrapLevel];
}

static void SetColourForGroup(GuiElement* gui, const std::string& colourDecName,
  const Colour& colour)
{
  GuiElement* elem = GetElementByName(gui, colourDecName);
  Assert(elem);
  if (!elem)
  {
    return;
  }
  GuiDecColour* decColour = dynamic_cast<GuiDecColour*>(elem);
  if (!decColour)
  {
    std::cout << "Unexpected type for " << colourDecName << "\n";
    Assert(false);
    return;
  }
  decColour->SetColour(colour);
  decColour->SetSecondColour(colour);
}

void Wallpaper::SetWallpaper(GuiElement* gui, int level)
{
  auto colours = GetColoursForLevel(level);
  SetColourForGroup(gui, "colour-wall-group-1", colours[0]);
  SetColourForGroup(gui, "colour-wall-group-2", colours[1]);
}
}

