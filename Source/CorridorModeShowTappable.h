// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "CorridorMode.h"

namespace Amju
{
// * CorridorModeShowTappable *
// Show tappable, which has a separate GUI.
class CorridorModeShowTappable : public CorridorMode
{
public:
  static const int ID = 3;
  virtual void OnActive() override;
  virtual void Update() override;
};

}
