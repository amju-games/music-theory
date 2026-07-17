#include <iostream>
#include <AmjuRand.h>
#include <Game.h>
#include <GameObjectFactory.h>
#include <LoadScene.h>
#include "AIFlee.h"
#include "AIWait.h"
#include "AnimalController.h"
#include "Describe.h"
#include "HeroGameRound.h"
#include "MySceneGraph.h"
#include "Palette.h"
#include "PFNpc.h"

namespace Amju
{
void AnimalController::Init(const HeroGameRound& gameRound)
{
  const std::string& pal = gameRound.m_palette;
  auto res = TheResourceManager::Instance()->GetRes(pal);
  if (!res)
  {
    return;
  }
  auto palette = dynamic_cast<Palette*>(res);
  if (!palette)
  {
    return;
  }
  m_pets = AddPetsForGameRound(*palette); 
  m_dino = AddAnimal("dino"); 
}

void AnimalController::Init()
{
  // TODO Make this its own more testable class?

  // Load any palette
  Palette palette;
  palette.Load("Image/palette-notes-12-2.png");
  m_pets = AddPetsForGameRound(palette); 
  // Initial AIs should come onto screen from the sides.

  // Dino for this round: we just need one, right?
  m_dino = AddAnimal("dino"); // Initial AI is to wait off screen.
}

void AnimalController::PetsFlee(Npc* dino)
{
  for (auto pet : m_pets)
  {
    if (!pet) continue;

    // within range?
    constexpr float FLEE_RADIUS = 100.f;
    // We only care about the x distance; we don't need to get
    //  vec sq len.
    const float dx = dino->GetPos().x - pet->GetPos().x;
#ifdef AI_DEBUG
std::cout << "Pet: " << Describe(pet) << ": dist: " << dx 
  << " / " << FLEE_RADIUS;
#endif

    if (std::abs(dx) < FLEE_RADIUS)
    {
#ifdef AI_DEBUG
std::cout << " -- FLEE!\n";
#endif
      AI* flee = pet->GetAI(AIFlee::NAME);
      flee->SetTarget(dino);
      pet->SetAI(flee);
    }
    else
    {
#ifdef AI_DEBUG
std::cout << "  no flee.\n";
#endif
    }
  }
}

void AnimalController::EatAPet(int petIndex)
{
  Assert(petIndex < static_cast<int>(m_pets.size()));
  auto pet = m_pets[petIndex];

  // Knock out the pet when eaten so we don't try to eat it again.
  if (!pet) return;
  m_pets[petIndex] = nullptr; // It's an RCPtr; there is still a ref to it.

  // Set dino z-track to that of the pet to be eaten.
  // Maybe simpler to just do this in the Chase AI.
  auto pos = m_dino->GetPos();
  pos.z = pet->GetPos().z;
  m_dino->SetPos(pos); 
 
  // Set dino AI to chase behaviour.
  // TODO different if we are on-screen doing something other than waiting.
  auto ai = m_dino->GetAI("chase");
  Assert(ai);
  ai->SetTarget(pet);
  m_dino->SetAI(ai);
}

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

std::vector<RCPtr<PFNpc>> AnimalController::AddPetsForGameRound(
  const Palette& palette)
{
  std::vector<RCPtr<PFNpc>> vec;

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

    // Add Wait AI so pets gradually appear.
    float waitTime = 5.f * (i + 1);
    auto wait = new AIWait(waitTime);
    pet->AddAI(wait);
    pet->SetAI(wait);
    // Once wait expires, wander on screen - so wander has high initial
    //  rank.

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

  auto pfnpc = dynamic_cast<PFNpc*>(go); 
  Assert(pfnpc);

  // For in-game animals, the camera is ortho and we can't see the shadows
  //  anyway. (In title mode we do want to see the shadows.)
  pfnpc->SetShadowVisible(false);

  return pfnpc;
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

