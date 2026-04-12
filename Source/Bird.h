#pragma once

#include "Npc.h"

namespace Amju
{
class Bird : public Npc
{
public:
  Bird();
  static const char* NAME;
  virtual const char* GetTypeName() const override;
  virtual void Update() override;
//  virtual void AddToGame() override;
};
}
