#pragma once

#include <GuiElement.h>

namespace Amju
{
class GuiDecAnimation;
class IExtra;

// * IReward *
// Rewards are visual displays of player getting a reward (for collecting
//  an Extra, but could be used for other things too.)
// They also set/increment/etc the appropriate value in the
//  game/player profile.
class IReward : public RefCounted
{
public:
  virtual ~IReward() = default;

  // Set the Extra which owns this reward
  void SetParent(IExtra* parent) { m_parent = parent; }

  // Called when anim finishes: actually update values.
  virtual void OnDestPosReached();

  // Called in ctor when dest pos set: initialise reward animation,
  //  and set to paused until we award.
  void InitAnim();

  // When awarded: start the anim to show the reward being given.
  void StartCollectAnim();

  // When not awarded (bum note etc.)
  void StartNoCollectAnim();

  // Called by owning Extra when the anim has finished. 
  // That control flow is a bit convoluted but lets the Extra decide what to do 
  //  when the anim ends, rather than hardcoding that behaviour. 
  virtual void GiveReward() = 0;

protected:
  // Root element for GUI that shows the reward on screen.
  // With the pointer back to Extra parent, we don't need to store the 
  //  gui twice.
  PGuiElement GetGui();

  // The Extra that owns this Reward.
  IExtra* m_parent = nullptr;

  // Anim controller: paused initially, started if/when we want to 
  //  give the reward. 
  GuiDecAnimation* m_animControllerCollect = nullptr;

  // Anim controller for if we don't collect
  GuiDecAnimation* m_animControllerNoCollect = nullptr;
  
  Vec2f m_destPos; // destination pos in screen coords - we set this
  //  when the reward is collected, then start the anim so the reward
  //  travels to the appropriate destination on screen. 
  // Set in ctor - presumably, we know the reward type and so where the
  //  gui should go on screen.
  // NB Query the position of GUI elements -- don't hard code any positions.
  // We could have landscape/portrait orientations.
};

using PReward = RCPtr<IReward>;

class RewardHealth : public IReward
{
public:
  RewardHealth(int points) { m_points = points; }
  void GiveReward() override;

private:
  int m_points;
};

class RewardPointsMult : public IReward
{
  RewardPointsMult(PGuiElement gui, int pointsMult, float duration);
  void GiveReward() override {}
};
}

