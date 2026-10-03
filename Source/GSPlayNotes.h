// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

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
  void Update() override;
  void OnActive() override;
};

typedef Singleton<GSPlayNotes> TheGSPlayNotes;
}

