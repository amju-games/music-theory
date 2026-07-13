#pragma once

#include <map>
#include <string>
#include <GameObject.h>
#include "AI.h"

namespace Amju
{
// MIXIN class for game entities which manage AIs.
// Controller decides which AI is currently active based on the rank
//  calculated by each AI.
class AIController
{
public:
  AIController();
  // TODO can't we use ints here?! Maybe this way it's easier to debug.
  AI* GetAI(const std::string& aiName);
  void SetAI(const std::string& aiName);
  void SetAI(AI*);

  void AddAI(AI*); // call to populate map
  void DecideAI();
  void UpdateAI();

  // Do we need?
  virtual void OnAnimFinished();
  virtual void OnAnimFreeze();
  virtual void OnAnimRepeat();

protected:
  using AIs = std::map<std::string, PAI>;
  AIs m_ais;
  AI* m_ai; // the current AI
};
}

