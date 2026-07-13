#include <Game.h>
#include <GameObjectFactory.h>
#include <LoadScene.h>
#include "AnimalController.h"
#include "MySceneGraph.h"
#include "PFNpc.h"

namespace Amju
{
GameObject* AnimalController::AddAnimal(const char* animalType)
{
  auto go = TheGameObjectFactory::Instance()->Create(animalType);
  if (!go)
  {
    std::cout << "Failed to create animal of type " << animalType << "!\n";
    return nullptr;
  }

  // Animals should load themselves, etc. in their ctor.
  // We are not loading a level from file in this game.
  TheGame::Instance()->AddGameObject(go);

  return go;
}

GameObject* AnimalController::AddAnimalFixedZ(const char* animalType, float z)
{
  auto go = AddAnimal(animalType);
  auto pfNpc = dynamic_cast<PFNpc*>(go);
  pfNpc->SetFixedZ(true, z);
  return pfNpc;
}

AnimalController& GetAnimalController()
{
  // TODO Setter for testing/mocking 
  static AnimalController ac;
  return ac;
}
}

