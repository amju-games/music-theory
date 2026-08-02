#include <AmjuAssert.h>
#include <GuiComposite.h>
#include "Extra.h"

namespace Amju
{
void IExtra::StartNoCollect()
{
  m_reward->StartNoCollectAnim();
}

void IExtra::StartCollection(
  GuiComposite* scrollingRoot, GuiComposite* nonScrollingRoot)
{
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
  m_reward->GiveReward();
}

void MultiEventExtra::Collect()
{
}

void ChildExtra::Collect() 
{
}
}

