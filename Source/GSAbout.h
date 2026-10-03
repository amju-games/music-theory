// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

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
