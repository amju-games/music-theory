#include <LoadScene.h>
#include "MySceneGraph.h"
#include "PFNpc.h"

namespace Amju
{
int PFNpc::CreateId()
{
  // Create new unique ID
  static int s_id = 0;
  return s_id++;
}

PFNpc::PFNpc()
{
  SetId(CreateId());
}

void PFNpc::Update() 
{
  Npc::Update();

  auto pos = GetPos();
  if (m_isZFixed)
  {
    pos.z = m_fixedZ;
    SetPos(pos);
  }

  UpdateShadow(pos);
}

void PFNpc::SetFixedZ(bool isFixed, float z) 
{
  m_isZFixed = isFixed; 
  m_fixedZ = z; 
}

void PFNpc::CreateSceneNode()
{
  // Find the root node to which we add all animals. 
  // Obvs this is very game-specific.
  auto sg = GetSceneGraph();
  auto root = sg->GetRootNode(SceneGraph::AMJU_OPAQUE); 
  auto parent = root->GetNodeByName("add-animals-to-me");
  Assert(parent);

  // Set scene filename from type name, avoiding duplication and
  //  source of error.
  std::string sceneFilename = 
    std::string("Scene/") + GetTypeName() + "-scene.txt";

  // Load scene tree specific to this animal's type. 
  m_sceneRoot = LoadScene(sceneFilename);
  if (!m_sceneRoot)
  {
    std::cout << "Failed to load scene: " << sceneFilename << "\n";
    Assert(0);
  }
  parent->AddChild(m_sceneRoot);

  // The tree should include the character MD2 and a shadow.
  // If we use generic names for these nodes, we can find them in
  //  generic code, otherwise pass in the node names we are 
  //  looking for.
  // (We need the md2 and shadow nodes separately.)
  auto md2 = dynamic_cast<Md2SceneNode*>(m_sceneRoot->GetNodeByName("md2"));
  Assert(md2);
  SetSceneNode(md2);
  // Set us up as the listener for animation events on the node.
  md2->SetListener(this); 

  SetShadowSceneNode(m_sceneRoot->GetNodeByName("shadow"));
}
}

