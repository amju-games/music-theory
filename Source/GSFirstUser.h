// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSBase.h"

namespace Amju
{
class GSFirstUser : public GSBase
{
public:
  GSFirstUser();
  virtual void OnActive() override;
};

using TheGSFirstUser = Singleton<GSFirstUser>;

}

