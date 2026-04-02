#pragma once

#include <Singleton.h>
#include "GSBaseWithHud.h"

namespace Amju
{
// Lose state: player did not successfully complete song in GSHero state.
class GSHeroEnd : public GSBaseWithHud
{
public:
  GSHeroEnd();

  void OnActive() override;
};

using TheGSHeroEnd = Singleton<GSHeroEnd>;
}

