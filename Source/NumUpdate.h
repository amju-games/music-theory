// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <string>
#include <GuiElement.h>

namespace Amju
{
// Update a number displayed in the HUD.
// Do animated scale, update drop shadow, if these exist.
void NumUpdate(PGuiElement gui, const std::string& elemName, int value);

void NumUpdate(PGuiElement gui, const std::string& elemName, const std::string& value);
}

