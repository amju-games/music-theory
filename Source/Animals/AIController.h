#pragma once

#include <map>
#include <string>
#include <GameObject.h>
#include "AI.h"
#include "AnimListener.h"

namespace Amju
{
// MIXIN class for game entities which manage AIs.
// Controller decides which AI to activate based on the rank
//  calculated by each AI.
class AIController : public AnimListener
{
public:
  AIController();
  virtual ~AIController() = default;

  // TODO can't we use ints here?! Maybe this way it's easier to debug.
  AI* GetAI(const std::string& aiName);
  void SetAI(const std::string& aiName);
  void SetAI(AI*);

  void AddAI(AI*); // call to populate map
  void DecideAI();
  void UpdateAI();

  // Called from MD2 controller: we notify the current AI.
  void OnAnimFinished() override;
  void OnAnimFreeze() override;
  void OnAnimRepeat() override;

protected:
  using AIs = std::map<std::string, PAI>;
  AIs m_ais;
  AI* m_ai; // the current AI
};
}

