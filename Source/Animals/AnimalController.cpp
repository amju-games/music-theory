#include <iostream>
#include <AmjuRand.h>
#include <Game.h>
#include <GameObjectFactory.h>
#include <LoadScene.h>
#include "AnimalController.h"
#include "MySceneGraph.h"
#include "Palette.h"
#include "PFNpc.h"

namespace Amju
{
void AnimalController::CleanUp()
{
  // Delete game objects.
  auto game = TheGame::Instance();
  auto numGos = game->GetGameObjects()->size();
  game->ClearGameObjects();
  Assert(game->GetGameObjects()->empty());
  std::cout << "Cleared up " << numGos << " game objects. Now there are none.\n";

  // Delete scene graph.
  ResetSceneGraph();
}

std::vector<PFNpc*> AnimalController::AddPetsForGameRound(
  const Palette& palette)
{
  std::vector<PFNpc*> vec;

  float z = 0;  // TODO Adjust this or the camera as required
  // Always 12? Or just the notes used in the game round?
  for (int i = 0; i < 12; ++i)
  {
    // TODO Need an algo here to give a mix of types depending on 
    //  game round unique ID. Also, do we want to distribute colours in a
    //  special way, e.g. cats always blues, squirrels always reds, etc???

    // Available pet types
    const std::array<std::string, 4>  PET_TYPES = 
      {{ "cat", "dog", "squirrel", "rabbit" }};

    // TODO srand on game round ID to make this the same every time for 
    //  any given game round
    int r = Amju::RandomInt(PET_TYPES.size());

    auto petType = PET_TYPES[r];
    auto pet = AddAnimalFixedZ(petType, z);
    z -= 40.f; // each pet is on a different 'track'

    // Colourise: set colour [i].
    pet->GetSceneNode()->SetColour(palette.GetColour(i));

    vec.push_back(pet);
  }
  
  return vec;
}

PFNpc* AnimalController::AddAnimal(const std::string& animalType)
{
  auto go = TheGameObjectFactory::Instance()->Create(animalType);
  if (!go)
  {
    std::cout << "Failed to create animal of type " << animalType << "! CHECK ANIMAL FACTORY AND TYPE NAME!\n";
    return nullptr;
  }

  // Animals should load themselves, etc. in their ctor.
  // We are not loading a level from file in this game.
  TheGame::Instance()->AddGameObject(go);

  return dynamic_cast<PFNpc*>(go); // I'd be surprised if that didn't work.
}

PFNpc* AnimalController::AddAnimalFixedZ(const std::string& animalType, float z)
{
  auto pfNpc = AddAnimal(animalType);
  Assert(pfNpc);
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

