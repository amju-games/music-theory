#include <AmjuAssert.h>
#include <GuiComposite.h>
#include "Extra.h"

namespace Amju
{
void IExtra::StartNoCollect()
{
  // Once-only check
  if (!m_isActive) return;
  m_isActive = false;

  Assert(m_reward);
  m_reward->StartNoCollectAnim();
}

void IExtra::FinishNoCollect()
{
  // Assuming direct parent of the GUI is the scrolling or non-scrolling
  //  root... even if not, we will detach from something :)
  auto comp = dynamic_cast<GuiComposite*>(m_gui->GetParent());
  if (!comp) return;  // we must have a parent... unless already detached?
  DetachGui(comp);

#ifdef EXTRA_DEBUG
std::cout << "Extra not collected, detached.\n";
#endif
}

void IExtra::StartCollection(
  GuiComposite* scrollingRoot, GuiComposite* nonScrollingRoot)
{
  // Once-only check
  if (!m_isActive) return;
  m_isActive = false;

  Assert(m_gui);
  auto pos = m_gui->GetCombinedPos(); // get local + scroll pos

  DetachGui(scrollingRoot); // detach from scrolling root

  m_gui->SetLocalPos(pos); // set local position so it looks the same

  // Add gui to non-scrolling root so it has a parent in the scene.
  nonScrollingRoot->AddChild(m_gui);

  // Notify reward to start animating to its destination pos
  m_reward->StartCollectAnim();
}

void IExtra::AttachToScrollingRoot(GuiComposite* root, const Vec2f& pos)
{
  root->AddChild(m_gui);

  // Move extra gui to desired pos on score
  m_gui->SetLocalPos(pos + m_gui->GetLocalPos());
}

void IExtra::DetachGui(GuiComposite* root)
{
  Assert(root);
  Assert(m_gui);
  int index = root->FindChildIndex(m_gui);
  Assert(index != -1); // root is not the direct parent of this extra!
  root->DeleteChild(index);
}

void Extra::Collect() 
{
  // Award points etc here
  Assert(m_reward);
  m_reward->GiveReward();
}
}

