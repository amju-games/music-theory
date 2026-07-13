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
}

