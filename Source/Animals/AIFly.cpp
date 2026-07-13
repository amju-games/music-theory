#include <iostream>
#include <DegRad.h>
#include "AIFly.h"
#include "Npc.h"

namespace Amju
{
const char* AIFly::NAME = "ai-fly";

const char* AIFly::GetName() const
{
  return NAME;
}

void AIFly::OnActivated()
{
  AI::OnActivated();
  Assert(m_npc);
  m_npc->SetAnim("fly"); 
}

void AIFly::Update()
{
  AI::Update();

  Assert(m_npc);

  Vec3f targetPos;
  if (m_target) 
  {
    targetPos = m_target->GetPos();
  }

  // Accelerate towards point above target
  Vec3f a = (targetPos + Vec3f(0, 50.0f, 0)) - m_npc->GetPos();
  a.Normalise();
  a *= 50.0f; // TODO CONFIG
  m_npc->SetAcc(a);

  // Cap speed 
  a = m_npc->GetVel();
  float speedSq = a.SqLen();
  const float MAX_SPEED = 50.0f; // TODO CONFIG
  if (speedSq > MAX_SPEED * MAX_SPEED)
  {
    a.Normalise();
    a *= MAX_SPEED;
    m_npc->SetVel(a);
  }

  float degs = RadToDeg(atan2(a.x, a.z));
  m_npc->SetDir(degs);
}
}

