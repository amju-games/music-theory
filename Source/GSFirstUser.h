// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "GSBase.h"

namespace Amju
{
// * GSFirstUser *
// Go to this state soon after start up when there has not been a user
//  previously. 
class GSFirstUser : public GSBase
{
public:
  GSFirstUser();
  virtual void OnActive() override;
};

using TheGSFirstUser = Singleton<GSFirstUser>;
}

