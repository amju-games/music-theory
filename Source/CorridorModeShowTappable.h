// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "CorridorMode.h"
#include "Tappable.h"

namespace Amju
{
// * CorridorModeShowTappable *
// Show tappable, which has a separate GUI.
class CorridorModeShowTappable : public CorridorMode
{
public:
  static const int ID;
  CorridorModeShowTappable();
  virtual void OnActive() override;
  virtual void OnDeactive() override;
  virtual void Update() override;
  virtual void Draw2d() override;

private:
  RCPtr<Tappable> m_tapped;
};

}
