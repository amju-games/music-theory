#pragma once

#include "Extra.h"

namespace Amju
{
// IExtra subtypes for collecting multiple connected extras

// For note runs etc, i.e. final extra is only collected when all child
//  extras are collected.
// Should this Extra be attached to the final note in the run, with
//  ChildExtras for the preceding ones? That sounds ok.
class MultiExtra : public Extra
{
public:
  MultiExtra(PGuiElement gui, PReward reward) : Extra(gui, reward) {}

  // Collect the multi extra! This might not need to be overridden.
  void Collect() override; 

  // No-collect this and all children.
  void StartNoCollect() override;

  // Add child extra, so we can notify them in StartNoCollect.
  void AddChild(PExtra child);

protected:
  // Child extras: when all are collected, we fire off Collect? No, we
  //  check they have all been collected when we are Collected.
  std::vector<PExtra> m_children;
};

// * Child Extra *
// This kind of extra is part of a collection that has one Multi Extra
//  parent. All the children and the parent must be collected for the
//  player to get the reward.
class ChildExtra : public Extra
{
public:
  ChildExtra(PGuiElement gui, PReward reward, MultiExtra* parent) :
    Extra(gui, reward), 
    m_parent(parent)
  {
    m_parent->AddChild(this);
  }

  // Mini reward and notify parent that this child was collected.
  // Maybe this one isn't necessary?? Or yes, bump up the points in the
  //  parent multi extra.
  void Collect() override;

  // Child not collected: notify the parent so we no-collect all the
  //  children and the parent.
  void StartNoCollect() override;

protected:
  MultiExtra* m_parent; 
};
}

