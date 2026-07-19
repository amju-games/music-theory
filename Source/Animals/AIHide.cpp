#include <AmjuRand.h>
#include "AIHide.h"
#include "BlinkSceneNode.h"
#include "Dino.h"

namespace Amju
{
const char* AIHide::NAME = "hide";

const char* AIHide::GetName() const
{
  return NAME;
}

void AIHide::Update()
{
  AI::Update();
  Assert(m_npc);

  // Stop once we get far enough to the side
  float x = m_npc->GetPos().x;
  if (std::abs(x) > PFNpc::OFF_SCREEN_X)
  {
    m_npc->SetAI("idle");
    m_npc->SetVisible(false);

    // Remove dino blood
    auto blinkNode = dynamic_cast<BlinkSceneNode*>(m_npc->GetSceneNode());
    Assert(blinkNode);
    blinkNode->LoadTextures(Dino::NO_BLOOD_TEX_1, Dino::NO_BLOOD_TEX_2);
  }
}

void AIHide::OnActivated()
{
  AI::OnActivated();

  Assert(m_npc);

  m_npc->SetAnim("walk");

  // Choose direction to run
  int runLeft = m_npc->GetPos().x < 0;
  const float SPEED = 75.f;
  m_npc->SetVel(Vec3f(runLeft ? -SPEED : SPEED, 0, 0));

  // Turn to face direction
  m_npc->SetDir(runLeft ? -90.f : 90.f);
}
}
