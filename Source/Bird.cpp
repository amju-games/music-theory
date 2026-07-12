#include <iostream>
#include <AmjuRand.h>
#include <DegRad.h>
#include <GameObjectFactory.h>
#include <LoadScene.h>
#include "AIFly.h"
#include "Bird.h"
#include "BlinkSceneNode.h"
#include "Describe.h"
#include "MySceneGraph.h"

namespace Amju
{
const char* Bird::NAME = "bird";

GameObject* CreateBird() { return new Bird; }

Bird::Bird()
{
  SetId(CreateId());

std::cout << "Creating new Bird: ID: " << GetId() << "\n";

  AddAI(new AIFly);

  m_sceneFilename = "Scene/bird-scene.txt";
  auto sg = GetSceneGraph();
  auto root = sg->GetRootNode(SceneGraph::AMJU_OPAQUE); 
  
  auto parent = root->GetNodeByName("add-animals-to-me");
  Assert(parent);

  CreateSceneNode(parent);

  Vec3f pos(Rnd(-10, 10) * 20, 100, Rnd(-10, 10) * 20);
  SetPos(pos);
}

const char* Bird::GetTypeName() const
{
  return NAME;
}

void Bird::Update()
{
  SetAI(GetAI(AIFly::NAME)); 

  Npc::Update();

  // Shadow
  auto pos = GetPos();
  // Set size depending on height from ground.
  // Max size is 1, min size 0
  const float maxH = 100.f;
  float shadowSize = (maxH - pos.y) / maxH;
  shadowSize = std::clamp(shadowSize, 0.f, 1.f);
  m_shadow.SetPosAndSize({pos.x, 0, pos.z}, shadowSize);

  // TODO Recalc collision vol
}

void Bird::CreateSceneNode(PSceneNode parent)
{
/*
  auto sn = new BlinkSceneNode;
  sn->LoadMd2("md2/bird.md2"); // TODO TEMP TEST
  sn->LoadTextures("Image/bird1.png", "Image/bird1a.png"); // TODO TEMP TEST
  m_sceneNode = sn;
  parent->AddChild(sn);
*/

  auto sn = LoadScene(m_sceneFilename);
  if (!sn)
  {
    std::cout << "Failed to load scene tree for bird!\n";
    Assert(0);
  }
  parent->AddChild(sn);

  auto birdMd2 = dynamic_cast<Md2SceneNode*>(sn->GetNodeByName("bird-md2"));
  SetSceneNode(birdMd2);

  m_shadow.SetSceneNode(sn->GetNodeByName("shadow"));
}
}

