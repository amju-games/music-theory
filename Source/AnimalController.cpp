#include <Game.h>
#include <GameObjectFactory.h>
#include <LoadScene.h>
#include "AnimalController.h"
#include "MySceneGraph.h"

namespace Amju
{
void AnimalController::AddAnimal(const char* animalType)
{
  auto go = TheGameObjectFactory::Instance()->Create(animalType);
  if (!go)
  {
    std::cout << "Failed to create animal of type " << animalType << "!\n";
    return ;
  }

  // Animals should load themselves, etc. in their factory func.
  TheGame::Instance()->AddGameObject(go);
}

AnimalController& GetAnimalController()
{
  // TODO Setter for testing/mocking 
  static AnimalController ac;
  return ac;
}
}

