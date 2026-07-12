#include <GameObjectFactory.h>
#include "AnimalFactory.h"
#include "Bird.h"

namespace Amju
{
void SetUpAnimalFactory()
{
  auto fac = TheGameObjectFactory::Instance();
  fac->Add(Bird::NAME, CreateBird);
}
}

