// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "CorridorMode.h"

namespace Amju
{
// Transition to entering classroom - game state changes after this.
// Display door opening, with camera moving towards/through it.
class CorridorModeEnterClassroom : public CorridorMode
{
public:
  static const int ID;
  virtual void Update() override;
  virtual void OnActive() override;

private:
  void UpdateOpeningDoor();

private:
  bool m_doorIsOpening = false;
  float m_doorAngleRads = 0;
};
}
