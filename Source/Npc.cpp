#include <File.h>
#include "Npc.h"

namespace Amju
{
int Npc::CreateId()
{
  // Create new unique ID
  static int s_id = 0;
  return s_id++;
}

void Npc::Update()
{
  GameObject::Update();

  // These type qualifiers aren't really necessary, I thought they
  //  might be good for readability
  AIController::UpdateAI();
  Md2SceneNodeController::UpdateMd2(GetPos());
}

bool Npc::Load(File* f)
{
  if (!GameObject::Load(f)) return false;

  // Load scene tree
 
  return true;
}

bool Npc::Load(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename)) return false; 
  return Load(&f);
}
}

