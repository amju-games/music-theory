#include <GameObjectFactory.h>
#include "AnimalFactory.h"
#include "Bird.h"
#include "Cat.h"
#include "Dino.h"
#include "Dog.h"
#include "Rabbit.h"
#include "Squirrel.h"

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
  AddToFactory<Cat>();
  AddToFactory<Dino>();
  AddToFactory<Dog>();
  AddToFactory<Rabbit>();
  AddToFactory<Squirrel>();
}
}

