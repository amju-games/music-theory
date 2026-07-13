#pragma once

namespace Amju
{
class AnimalController
{
public:
  // Add one animal: typeName needs to match the name registered with
  //  GameObjectFactory.
  // Adds new game object to the game but also returns it.
  // This animal can move freely so would be for the title state where
  //  we have a persp camera.
  GameObject* AddAnimal(const char* typeName);

  // Add animal which moves only along a line parallel to the x-axis.
  // This is for hero mode, where we have an ortho camera.
  GameObject* AddAnimalFixedZ(const char* typeName, float z);
};

AnimalController& GetAnimalController();
}

