// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "CorridorMode.h"

namespace Amju
{
// * CorridorModeExitTappable *
// Transition back from focusing on a tappable, to re-entering Wait mode
class CorridorModeExitTappable : public CorridorMode
{
public:
  static const int ID = 5;
  virtual void Update() override;
};

}
