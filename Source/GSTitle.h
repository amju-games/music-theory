// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSBase.h"

namespace Amju
{
// * GSTitle*
// Show title page
class GSTitle : public GSBase
{
public:
  GSTitle();
  virtual void OnActive() override;
};

typedef Singleton<GSTitle> TheGSTitle;
}



