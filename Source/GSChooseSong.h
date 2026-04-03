#pragma once

#include "GSBase.h"

namespace Amju
{
class GSChooseSong : public GSBase
{
public:
  GSChooseSong();

  void OnActive() override;

protected:
  void InitGui();
};

using TheGSChooseSong = Singleton<GSChooseSong>;
}

