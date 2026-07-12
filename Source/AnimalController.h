#pragma once

namespace Amju
{
class AnimalController
{
public:
  // Add one animal: typeName needs to match the name registered with
  //  GameObjectFactory.
  void AddAnimal(const char* typeName);
};

AnimalController& GetAnimalController();
}

