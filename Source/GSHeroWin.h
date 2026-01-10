#pragma once

#include <Singleton.h>
#include "GSBase.h"

namespace Amju
{
class GSHeroWin : public GSBase
{
public:
  GSHeroWin();

  void OnActive() override;
};

using TheGSHeroWin = Singleton<GSHeroWin>;
}

