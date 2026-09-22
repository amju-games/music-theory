#include <iostream>
#include <AmjuAssert.h>
#include <GuiDecAnimation.h>
#include "MultiExtra.h"

//#define MULTI_EXTRA_DEBUG

namespace Amju
{
void BoostablePointsExtra::BoostPoints(int pointsToAdd)
{
  auto rp = dynamic_cast<RewardPoints*>(GetReward().GetPtr());
  int totalPoints = pointsToAdd + rp->GetPoints();
  rp->SetPoints(totalPoints);

  auto text = " +" + std::to_string(totalPoints);
  SetRewardGuiText(GetGui(), text);

  // Trigger pulse anim
  auto anim = dynamic_cast<GuiDecAnimation*>(GetGui()->
      GetElementByName("pulsing-anim-delay"));
  if (anim)
  {
    anim->ResetAnimation();
  }
}

MultiExtra::MultiExtra(PGuiElement gui, PReward reward) 
  : BoostablePointsExtra(gui, reward) 
{
}

void MultiExtra::Collect() 
{
  // Collect the multi extra! This might not need to be overridden.
  Extra::Collect();
}

void MultiExtra::StartNoCollect() 
{
#ifdef MULTI_EXTRA_DEBUG
std::cout << "No collect on multi extra..\n";
#endif

  // No-collect this and all children.
  Extra::StartNoCollect(); 

  for (auto child : m_children)
  {
    if (child->IsActive())
    {
      child->StartNoCollect();
    }
  }
}

void MultiExtra::AddChild(PExtra child)
{
  // Add child extra, so we can notify them in StartNoCollect.
  m_children.push_back(child);
}

void ChildExtra::Collect()
{
  // Boost points for next element, which could be another child
  //  or the final 'parent' multi-extra.
  Assert(m_parent);

  // Get points for this element, to add to the next one.
  auto pointsReward = dynamic_cast<RewardPoints*>(m_reward.GetPtr());
  Assert(pointsReward);
  int pointsToAdd = pointsReward->GetPoints();

  // Add the points to the next element
  if (m_nextExtra == m_parent)
  {
    m_parent->BoostPoints(pointsToAdd);
  }
  else
  {
    auto next = dynamic_cast<ChildExtra*>(m_nextExtra);
    next->BoostPoints(pointsToAdd);
    // Also show total in parent?
    m_parent->BoostPoints(pointsToAdd);
  }

  Extra::Collect();
}

void ChildExtra::StartNoCollect() 
{
#ifdef MULTI_EXTRA_DEBUG
std::cout << "No collect on child  extra..\n";
#endif

  // Child not collected: notify the parent so we no-collect all the
  //  children and the parent.
  Assert(m_parent);
  bool isParentActive = m_parent->IsActive();

  Extra::StartNoCollect();

  if (isParentActive)
  {
    m_parent->StartNoCollect();
  }
}
}

