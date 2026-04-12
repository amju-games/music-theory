#pragma once

#include "AI.h"

namespace Amju
{
class AIIdle : public AI
{
public:
  static const char* NAME;

  AIIdle();
  virtual void Update();
  virtual const char* GetName() const;
  virtual void OnActivated();
};
}

