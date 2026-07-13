#pragma once

#include "PFNpc.h"

namespace Amju
{
class Bird : public PFNpc
{
public:
  Bird();
  static const char* NAME;
  virtual const char* GetTypeName() const override;
  virtual void Update() override;
  void CreateSceneNode(PSceneNode parent) override;
};

GameObject* CreateBird();
}
