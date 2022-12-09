// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "CorridorModeShowTappable.h"

namespace Amju
{
void CorridorModeShowTappable::Update()
{
  CorridorMode::Update();

}

void CorridorModeShowTappable::OnActive()
{
  CorridorMode::OnActive();

  // Load GUI: back button, and Tappable has its own custom GUI
}

}
