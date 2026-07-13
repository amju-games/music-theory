#include <GameObjectFactory.h>
#include "AnimalFactory.h"
#include "Bird.h"
#include "Dino.h"

namespace Amju
{
void SetUpAnimalFactory()
{
  auto factory = TheGameObjectFactory::Instance();

  factory->Add(Bird::NAME, CreateBird);
  factory->Add(Dino::NAME, CreateDino);
}
}

