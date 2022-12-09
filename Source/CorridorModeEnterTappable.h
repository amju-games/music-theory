// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <Vec3.h>
#include "CorridorMode.h"

namespace Amju
{
// * CorridorModeEnterTappable *
// Transition to showing tappable
class CorridorModeEnterTappable : public CorridorMode
{
public:
  static const int ID = 4;
  virtual void OnActive() override;
  virtual void Update() override;

private:
  float m_camLerpTime = 0;
};

}
