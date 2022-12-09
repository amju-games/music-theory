// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSBase.h"

namespace Amju
{
class GSBase3d : public GSBase
{
public:
  virtual void Update() override;
  virtual void Draw() override;
  virtual bool OnKeyEvent(const KeyEvent& ke) override;

protected:
  virtual void Load3d();
};
}
