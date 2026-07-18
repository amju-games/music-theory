#pragma once

#include "AI.h"

namespace Amju
{
class AIIdle : public AI
{
public:
  static const char* NAME;

  AIIdle();
  const char* GetName() const override;
  void OnActivated() override;
};
}

