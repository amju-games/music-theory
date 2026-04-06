#pragma once

#include "GSShowGui.h"

namespace Amju
{
// ** Choose song **
// We are building a GUI in code, and to look at extents, we
//  inherit from GSShowGui.
class GSChooseSong : public GSShowGui
{
public:
  GSChooseSong();

  void OnActive() override;
  void Draw2d() override;

protected:
  void InitGui();
};

using TheGSChooseSong = Singleton<GSChooseSong>;
}

