#include <Game.h>
#include <GameObjectFactory.h>
#include <LoadScene.h>
#include "AnimalController.h"
#include "MySceneGraph.h"

namespace Amju
{
void AnimalController::AddAnimal(const char* animalType)
{
/*
  std::string sceneFilename = "Scene/bird-scene.txt";
//  std::string sceneFilename = "Scene/3d-title-scene.txt";
  auto sg = GetSceneGraph();
  auto root = sg->GetRootNode(SceneGraph::AMJU_OPAQUE); 
  auto parent = root->GetNodeByName("add-animals-to-me");
  Assert(parent);

  auto sn = LoadScene(sceneFilename);
  if (!sn)
  {
    std::cout << "Failed to load scene tree for bird!\n";
    Assert(0);
  }
  parent->AddChild(sn);
*/

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

