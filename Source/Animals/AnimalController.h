#pragma once

#include <string>
#include <vector>

namespace Amju
{
class Palette;
class PFNpc;

class AnimalController
{
public:
  // Add one animal: typeName needs to match the name registered with
  //  GameObjectFactory.
  // Adds new game object to the game and returns its dynamic type.
  // This animal can move freely so would be for the title state where
  //  we have a persp camera.
  PFNpc* AddAnimal(const std::string& typeName);

  // Add animal which moves only along a line parallel to the x-axis.
  // This is for hero mode, where we have an ortho camera.
  PFNpc* AddAnimalFixedZ(const std::string& typeName, float z);

  std::vector<PFNpc*> AddPetsForGameRound(
    const Palette& palette);
};

AnimalController& GetAnimalController();
}

