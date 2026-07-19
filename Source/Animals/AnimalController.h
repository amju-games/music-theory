#pragma once

#include <string>
#include <vector>
#include "PFNpc.h"

namespace Amju
{
struct HeroGameRound;
class Palette;

class AnimalController
{
public:
  // Initialise animals 
  void Init(); // for testing
  void Init(const HeroGameRound&); // for in-game

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

  // Set dino after the given pet, if it's not already eaten and not in
  //  wait state.
  void EatAPet(int petIndex);

  // Called when the pet has been eaten: we need this in case a pet
  //  we were going to eat gets passed over. We call this when the
  //  pet is definitely eaten.
  void SetPetEaten(int petIndex);

  // Called once dino starts eating a pet: other pets within range
  //  can flee (TODO they could be sick, etc.)
  void PetsFlee(Npc* dino);

  // Call at end of round: surviving pets jump up 
  void PetsJump();

protected:
  std::vector<RCPtr<PFNpc>> m_pets;
  RCPtr<PFNpc> m_dino;
};

AnimalController& GetAnimalController();
}

