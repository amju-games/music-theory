// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
// * GSPlayNotes *
// Test game state: plays midi notes
class GSPlayNotes : public GSBase
{
public:
  GSPlayNotes();
  virtual void Update() override;
};

typedef Singleton<GSPlayNotes> TheGSPlayNotes;
}

