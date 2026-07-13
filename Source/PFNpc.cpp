#include <LoadScene.h>
#include "MySceneGraph.h"
#include "PFNpc.h"

namespace Amju
{
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

  // Load scene tree specific to this animal's type. 
  // The tree should include the character MD2 and a shadow.
  // If we use generic names for these nodes, we can find them in
  //  generic code, otherwise pass in the node names we are 
  //  looking for.
  // We need the md2 and shadow nodes separately.
  auto sn = LoadScene(m_sceneFilename);
  if (!sn)
  {
    std::cout << "Failed to load scene: " << m_sceneFilename << "\n";
    Assert(0);
  }
  parent->AddChild(sn);

  auto md2 = dynamic_cast<Md2SceneNode*>(sn->GetNodeByName("md2"));
  Assert(md2);
  SetSceneNode(md2);

  SetShadowSceneNode(sn->GetNodeByName("shadow"));
}
}

