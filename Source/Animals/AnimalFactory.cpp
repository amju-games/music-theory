#include <GameObjectFactory.h>
#include "AnimalFactory.h"
#include "Bird.h"
#include "Dino.h"

namespace Amju
{
template <class ANIMAL>
static void AddToFactory()
{
  auto factory = TheGameObjectFactory::Instance();
  factory->Add(ANIMAL::NAME, []() -> GameObject* { return new ANIMAL; });
}

void SetUpAnimalFactory()
{
  AddToFactory<Bird>();
  AddToFactory<Dino>();
}
}

