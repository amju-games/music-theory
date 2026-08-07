#pragma once

#include <GuiElement.h>
#include "Reward.h"

namespace Amju
{
class GuiComposite;

// * IExtra *
// Interface for Extras.
// Subtypes will behave differently when collected by the player.
// Also subtypes will have different rules for how they are attached
//  to glyphs in the music score, (one note, or a run?) 
//  *and* how we allocate them throughout
//  the piece. (E.g. every 10 notes? Randomly? At the end, etc.)
class IExtra : public RefCounted
{
public:
  virtual ~IExtra() = default;

  // After create: attach gui to the scrolling score, at the
  //  given position.
  void AttachToScrollingRoot(GuiComposite* scrollingRoot, const Vec2f& pos);

  // When extra is to be collected: detach from scrolling root,
  //  attach to non-scrolling root.
  void StartCollection(
    GuiComposite* scrollingRoot, GuiComposite* nonScrollingRoot);

  // When collection anim has finished: award points etc.
  virtual void Collect() = 0;

  // When extra is not collected (bum note etc.)
  void StartNoCollect(); 

  // When no collect anim completes, this is called, for final cleanup.
  void FinishNoCollect();

  // For the Reward we own to access the gui.
  PGuiElement GetGui() { return m_gui; }

  // Detach the Extra from the given parent node, which has to be
  //  the direct parent, not some other ancestor.
  void DetachGui(GuiComposite* root);

protected:
  // This is what the Extra looks like.
  PGuiElement m_gui;

  // This is the Reward given to the player when collected.
  PReward m_reward;

  // Reset once we first collect or un-collect, so we don't try to do it 
  //  more than once.
  bool m_isActive = true; 
};

using PExtra = RCPtr<IExtra>;

// Standard 1:1 event ID to reward
class Extra : public IExtra
{
public:
  Extra(PGuiElement gui, PReward reward)
  {
    m_gui = gui;
    m_reward = reward;
    m_reward->SetParent(this);
    m_reward->InitAnim();
  }

  void Collect() override; 
};

// For note runs etc, i.e. final extra is only collected when all child
//  extras are collected.
// Should this Extra be attached to the final note in the run, with
//  ChildExtras for the preceding ones? That sounds ok.
class MultiEventExtra : public IExtra
{
public:
  // Collect won't be called externally, right? We do it ourself when the
  //  last child is collected and all children have been.
  void Collect() override; 

  // Child extras: when all are collected, we fire off Collect? No, we
  //  check they have all been collected when we are Collected.
  std::vector<PExtra> m_children;
};

// Used internally in above type and probably should be an inner class of 
//  MultiEventExtra. Keep external for now, maybe there will be other
//  composite types?
class ChildExtra : public IExtra
{
public:
  // Mini reward and notify parent that this child was collected.
  void Collect() override;

  MultiEventExtra* m_parent = nullptr;
};
}

