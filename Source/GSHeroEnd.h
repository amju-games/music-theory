#pragma once

#include <Singleton.h>
#include "GSBase.h"
#include "WithHud.h"

namespace Amju
{
// Lose state: player did not successfully complete song in GSHero state.
class GSHeroEnd : public GSBase, public WithHud
{
public:
  GSHeroEnd();

  void Update() override;
  void OnActive() override;
};

using TheGSHeroEnd = Singleton<GSHeroEnd>;
}

