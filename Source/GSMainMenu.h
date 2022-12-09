// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
class GSMainMenu : public GSBase
{
public:
  GSMainMenu();
  virtual void OnActive() override;
};

typedef Singleton<GSMainMenu> TheGSMainMenu;
}

