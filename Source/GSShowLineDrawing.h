// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
class GSShowLineDrawing : public GSBase
{
public:
  virtual void Update() override;
  virtual void OnActive() override;

  // Reload
  virtual bool OnKeyEvent(const KeyEvent&) override;
};

typedef Singleton<GSShowLineDrawing> TheGSShowLineDrawing;
}

