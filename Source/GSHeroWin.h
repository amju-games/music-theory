#pragma once

#include <Singleton.h>
#include "GSBaseWithHud.h"

namespace Amju
{
// WIN state, following successful song rendition in GSHero
class GSHeroWin : public GSBaseWithHud
{
public:
  GSHeroWin();

  void OnActive() override;
};

using TheGSHeroWin = Singleton<GSHeroWin>;
}

