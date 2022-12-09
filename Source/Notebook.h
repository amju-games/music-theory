// * Amjula music theory *
// (c) Copyright 2017-2018 Jason Colman

#pragma once

#include "LurkMsg.h"

namespace Amju
{
class Notebook : public LurkMsg
{
public:
  Notebook();

  void Draw() override;
  void Update() override;
};
}
