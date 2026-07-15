#pragma once

#include <string>
#include <vector>
#include <PFNpc.h>

namespace Amju
{
class Palette;

class AnimalController
{
public:
  void Init();

  // Clean up game objects and scene graph.
  void CleanUp();

  // Add one animal: typeName needs to match the name registered with
  //  GameObjectFactory.
  // Adds new game object to the game and returns its dynamic type.
  // This animal can move freely so would be for the title state where
  //  we have a persp camera.
  PFNpc* AddAnimal(const std::string& typeName);

  // Add animal which moves only along a line parallel to the x-axis.
  // This is for hero mode, where we have an ortho camera.
  PFNpc* AddAnimalFixedZ(const std::string& typeName, float z);

  std::vector<RCPtr<PFNpc>> AddPetsForGameRound(
    const Palette& palette);

  void EatAPet(int petIndex);

protected:
  std::vector<RCPtr<PFNpc>> m_pets;
  RCPtr<PFNpc> m_dino;
};

AnimalController& GetAnimalController();
}

