#include <AmjuAssert.h>
#include "Npc.h"

namespace Amju
{
void Npc::Update()
{
  GameObject::Update();

  // These type qualifiers aren't really necessary, I thought they
  //  might be good for readability
  AIController::UpdateAI();
  Md2SceneNodeController::UpdateMd2(GetPos());
}

void Npc::SetVisible(bool isVisible)
{
  Assert(m_sceneRoot); // Not set yet!
  m_sceneRoot->SetVisible(isVisible);
}
}

