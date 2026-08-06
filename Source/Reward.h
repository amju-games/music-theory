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

  // Called from callback when not-collected anim completes.
  // We detach the GUI from its root so it is deleted.
  void OnNoCollectAnimComplete();

  // Called by owning Extra when the anim has finished. 
  // That control flow is a bit convoluted but lets the Extra decide what to do 
  //  when the anim ends, rather than hardcoding that behaviour. 
  virtual void GiveReward() const = 0;

  // Get destination pos in screen coords for the collection anim.
  // NB Query the position of GUI elements -- don't hard code any positions.
  // We could have landscape/portrait orientations.
  // Called as late as poss to minimise visual bugs due to the orientation changing.
  // (Very low priority proper fix: register for notification when orientation
  //  changes, if/when we add this.)
  virtual Vec2f GetCollectDestPos() const = 0;

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

// * Reward Health *
// Give player some extra health/life.
class RewardHealth : public IReward
{
public:
  RewardHealth(int points) { m_points = points; }

  // Increase the player health by m_points, up to max.
  void GiveReward() const override;

  // Get screen coords of player health indicator (query GSHero gui).
  Vec2f GetCollectDestPos() const override;

private:
  int m_points;
};

// * Reward Points Mult *
// Player gets additions to points multiplied by some value for a duration.
// (The multipliers should themselves multiply, so e.g. a x2 followed by another
//  x2 within the duration, gives x4. But that's not done in this class.) 
class RewardPointsMult : public IReward
{
public:
  RewardPointsMult(int pointsMult) { m_pointsMult = pointsMult; }

  // Multiply current player score multiplier by our member value.
  void GiveReward() const override;

  // Get screen coords of player score multiplier
  Vec2f GetCollectDestPos() const override;

private:
  int m_pointsMult;
};
}

