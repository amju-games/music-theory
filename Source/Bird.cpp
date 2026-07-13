#include <iostream>
#include <AmjuRand.h>
#include "AIFly.h"
#include "Bird.h"

namespace Amju
{
const char* Bird::NAME = "bird";

GameObject* CreateBird() { return new Bird; }

Bird::Bird()
{
  SetId(CreateId());

std::cout << "Creating new Bird: ID: " << GetId() << "\n";

  m_sceneFilename = "Scene/bird-scene.txt";
  CreateSceneNode();

  Vec3f pos(Rnd(-10, 10) * 20, 100, Rnd(-10, 10) * 20);
  SetPos(pos);

  auto ai = new AIFly;
  AddAI(ai);
  SetAI(ai); // The only AI for this type.. right? 
}

const char* Bird::GetTypeName() const
{
  return NAME;
}
}

