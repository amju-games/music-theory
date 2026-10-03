// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

#pragma once

#include "GSBase.h"

namespace Amju
{
// * GSVerify *
// Verify that all midi files load ok and have expected channels, etc.
// Set as initial game state.
class GSVerify : public GSBase
{
public:
  GSVerify();
  void Update() override;
};

using TheGSVerify = Singleton<GSVerify>;
}

