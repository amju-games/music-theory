// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "CorridorMode.h"

namespace Amju
{
// * CorridorModeExitClassroom *
// Transition to Wait mode, after leaving a classroom
class CorridorModeExitClassroom : public CorridorMode
{
public:
  static const int ID;
  virtual void Update() override;
};
}
