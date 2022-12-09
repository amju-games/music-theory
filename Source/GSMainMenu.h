// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSBase3d.h"
#include <Singleton.h>

namespace Amju
{
class GSMainMenu : public GSBase3d
{
public:
  GSMainMenu();
  virtual void OnActive() override;
  void GoToTopic(int topic);
};

typedef Singleton<GSMainMenu> TheGSMainMenu;
}

