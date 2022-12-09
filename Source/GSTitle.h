// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "GSBase.h"

namespace Amju
{
// * GSTitle*
// Show title, option buttons
class GSTitle : public GSBase
{
public:
  GSTitle();
  virtual void OnActive() override;
};

typedef Singleton<GSTitle> TheGSTitle;
}



