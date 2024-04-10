// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "GSBase.h"

namespace Amju
{
// * GSAbout*
// About page, with button which returns you to prev state
class GSAbout : public GSBase
{
public:
  GSAbout();
  virtual void OnActive() override;
};

typedef Singleton<GSAbout> TheGSAbout;
}
