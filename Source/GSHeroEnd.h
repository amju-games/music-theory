#pragma once

#include <Singleton.h>
#include "GSBase.h"

namespace Amju
{
class GSHeroEnd : public GSBase
{
public:
  GSHeroEnd();

  void OnActive() override;
};

using TheGSHeroEnd = Singleton<GSHeroEnd>;
}

