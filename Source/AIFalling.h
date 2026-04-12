#ifndef AI_FALLING_H
#define AI_FALLING_H

#include "AI.h"

namespace Amju
{
class AIFalling : public AI
{
public:
  static const char* NAME;
  AIFalling();
  virtual const char* GetName() const;
  virtual void Update();
};
}

#endif
