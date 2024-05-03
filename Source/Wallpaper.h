// * Amjula music theory * (c) Copyright 2024 Juliet Colman

#pragma once

namespace Amju
{
class GuiElement;

// * Wallpaper *
// Mixin class for Game States that display wallpaper, so we can 
//  set the colours consistently.
class Wallpaper
{
public:
  // Set wallpaper colours.
  // guiRoot should have Colour decorator descendants "colour-wall-group-1" and
  //  "colour-wall-group-2".
  void SetWallpaper(GuiElement* guiRoot, int level);
};
}

