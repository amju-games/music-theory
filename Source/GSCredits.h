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
};

using TheGSCredits = Singleton<GSCredits>;

void OnCreditsButton(GuiElement* elem);
}

