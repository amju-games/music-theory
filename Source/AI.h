#pragma once

#include <RCPtr.h>

namespace Amju
{
class Npc;
class GameObject;

// Base class for AI states for NPCs
class AI : public RefCounted
{
public:
  virtual const char* GetName() const = 0;

  // Updates timer
  virtual void Update() = 0;

  // Resets timer
  virtual void OnActivated();

  virtual void OnDeactivated() {}

  // Return float - higher means this AI is better
  virtual float GetRank();

  void SetNpc(Npc* npc);
  // Most AI behaviours involve some other target, e.g. to chase or avoid
  void SetTarget(GameObject*);

  // Called from scene node which controls animation
  virtual void OnAnimFinished() {}
  virtual void OnAnimFreeze() {}
  virtual void OnAnimRepeat() {}

protected:
  Npc* m_npc = nullptr;
  GameObject* m_target = nullptr;
  float m_time = 0; // time since activated
  float m_rank = 0; // default rank 
};

using PAI = RCPtr<AI>;
}

