#pragma once

#include <Singleton.h>
#include "GSBase.h"
#include "WithHud.h"

namespace Amju
{
// WIN state, following successful song rendition in GSHero
class GSHeroWin : public GSBase, public WithHud
{
public:
  GSHeroWin();

  void Update() override;
  void OnActive() override;
};

using TheGSHeroWin = Singleton<GSHeroWin>;
}

