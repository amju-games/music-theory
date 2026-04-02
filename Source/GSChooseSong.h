#pragma once

#include "GSBase.h"

namespace Amju
{
class GSChooseSong : public GSBase
{
public:
  GSChooseSong();

  void OnActive() override;
};

using TheGSChooseSong = Singleton<GSChooseSong>;
}

