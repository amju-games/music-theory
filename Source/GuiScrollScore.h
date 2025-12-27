#pragma once

#include "GuiMusicScore.h"

namespace Amju
{
// * GuiScrollScore *
// Scroll score from right to left, rather than static/highlighting.
class GuiScrollScore : public GuiMusicScore
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  virtual void Animate(float animValue) override;
  virtual void OnResetAnimation() override;
};
}

