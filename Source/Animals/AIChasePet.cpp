#include <cmath>
#include <iostream>
#include <GameObject.h>
#include <DegRad.h>
#include "AIChasePet.h"
#include "AngleVec.h"
#include "Describe.h"
#include "PFNpc.h"

namespace Amju
{
const char* AIChasePet::NAME = "chase";

const char* AIChasePet::GetName() const
{
  return NAME;
}

void AIChasePet::OnActivated()
{
  Assert(m_npc);
  Assert(m_target);

  m_npc->SetVisible(true);
}

void AIChasePet::Update()
{
  AI::Update();

  Assert(m_npc);
  Assert(m_target);

  // Head towards target
  Vec3f aim = m_target->GetPos();
  Vec3f vel = aim - m_npc->GetPos();
  vel.y = 0; // we don't want y diff due to bouncing

  // Turn to face target, use convenience function wrapping atan2
  m_npc->SetDir(GetAngleDegsFromVec(vel)); 

  static const float MAX_DIST = 40.f;
  static const float MAX_DIST_SQ = MAX_DIST * MAX_DIST;

  float sqlen = vel.SqLen();
  if (sqlen < MAX_DIST_SQ)
  {
#ifdef AI_DEBUG
std::cout << "AI chase: " << Describe(m_npc) << " has reached target " <<
  Describe(m_target) << "!\n";
#endif

    m_npc->SetVel(Vec3f(0, -500.f, 0)); // fall back to ground

    auto eatAI = m_npc->GetAI("eat");
    eatAI->SetTarget(m_target);
    m_npc->SetAI(eatAI); 
  }
  else 
  {
    vel.Normalise();

    static const float SPEED = 150.f; 
    vel *= SPEED;
    Vec3f v = m_npc->GetVel();
    v.x = vel.x;
    v.y = 0; //std::sin(m_time * 20.f) * 300.f; // bob up and down
    v.z = vel.z;
    m_npc->SetVel(v);
    m_npc->SetAnim("run");
  }
}
}

