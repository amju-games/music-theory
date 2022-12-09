// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
class GSShowLineDrawing : public GSBase
{
public:
  virtual void OnActive() override;
};

typedef Singleton<GSShowLineDrawing> TheGSShowLineDrawing;
}

