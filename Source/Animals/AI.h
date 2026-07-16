#pragma once

#include <RCPtr.h>

namespace Amju
{
class GameObject;
class Npc;

// Base class for AI states for NPCs
class AI : public RefCounted
{
public:
  virtual const char* GetName() const = 0;

  // Default behaviour updates timer.
  virtual void Update();

  // Called if max time is set and we reached it.
  // Default behaviour is to Decide on a new best AI.
  virtual void OnMaxTimeReached();

  // Resets timer
  virtual void OnActivated();

  virtual void OnDeactivated() {}

  // Return float - higher means this AI is better
  virtual float GetRank();

  // Set the AI controller, which decides when to activate/deactivate this AI.
  void SetNpc(Npc* npc);

  // Most AI behaviours involve some other target, e.g. to chase or avoid
  void SetTarget(GameObject*);

  virtual void OnAnimFinished() {} // e.g. decide on new AI once anim ends
  virtual void OnAnimFreeze() {}
  virtual void OnAnimRepeat() {}

protected:
  Npc* m_npc = nullptr; // NPC this AI directs
  GameObject* m_target = nullptr; // target of this behaviour
  float m_time = 0; // time since activated
  float m_maxTime = 0; // time until we decide new behaviour
  float m_rank = 0; // default rank 
};

using PAI = RCPtr<AI>;
}

