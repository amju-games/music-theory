#pragma once

#include "GSBase3d.h"
#include <Singleton.h>

namespace Amju
{
// * GSAnimals *
// Test game state: shows animals, allows for testing.
// This is the start state if you specify --animals on the command line.
class GSAnimals : public GSBase3d
{
public:
  GSAnimals();

  void Update() override;
  void OnActive() override;
  void OnDeactive() override;

  // Press keys to add animals
  KeyInputHandler& AddKeyInputHandlers() override;
};

using TheGSAnimals = Singleton<GSAnimals>;
}

