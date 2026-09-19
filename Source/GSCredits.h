#pragma once

#include "GSBase3d.h"

namespace Amju
{
// * GSCredits *
// Show credits; possibly with 3D graphics
class GSCredits : public GSBase3d
{
public:
  GSCredits();
  void OnActive() override;
  void Update() override;
  void AutoTestSetup() override;

private:
  float m_waitTime;
  float m_scrollVel;
};

using TheGSCredits = Singleton<GSCredits>;

void OnCreditsButton(GuiElement* elem);
}

