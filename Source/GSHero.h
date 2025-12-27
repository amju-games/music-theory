// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
// * GSHero *
// Hero mode: the game mechanic which is like Guitar Hero, but for keyboard/sheet music.
class GSHero : public GSBase
{
public:
  GSHero();

  void OnActive() override;
};

typedef Singleton<GSHero> TheGSHero;
}
