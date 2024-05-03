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
  void SetWallpaper(GuiElement* gui, int level);
};
}

