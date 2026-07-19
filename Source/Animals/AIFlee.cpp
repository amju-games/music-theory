#include <AmjuRand.h>
#include <Timer.h>
#include "AIFlee.h"
#include "AngleVec.h"
#include "PFNpc.h"

namespace Amju
{
const char* AIFlee::NAME = "flee";

AIFlee::AIFlee()
{
  // Note on Rank: we don't choose this behaviour, it gets set
  //  by AnimalController::PetFlee. So the rank should be very low.
  m_rank = -1000.f;
}

const char* AIFlee::GetName() const { return NAME; }

void AIFlee::OnMaxTimeReached()
{
  // Go to Idle state, to avoid immediately Wandering back towards 
  //  the dino.
  m_npc->SetAI("idle");

  // Turn to face player accusingly
  m_npc->SetDir(Rnd(-30.f, 30.f));
}

void AIFlee::Update() 
{
  AI::Update();
  StayOnScreen();
}

void AIFlee::OnActivated() 
{
  AI::OnActivated();

  // Decide on direction to go, and how long for.
  m_maxTime = 1.5f + Rnd(0, 1.f);

  // Prefer to not wander off screen. So if we are near the edge,
  //  restrict the direction we can go in.
  float dir = 0;
  float x = m_npc->GetPos().x;
  const float EDGE = PFNpc::OFF_SCREEN_X * .7f; 
  if (x < -EDGE)
  {
    dir = 90.f; 
  }
  else if (x > EDGE)
  {
    dir = -90.f; 
  }
  else
  {
    // Run away from target (the dino) - just in x?
    auto vec = m_npc->GetPos() - m_target->GetPos();
    // Reduce z so we run across screen more.
    vec.z *= .2f;
    dir = GetAngleDegsFromVec(vec); 
  }
#ifdef AI_DEBUG
std::cout << "Flee dir: " << dir << " degs\n";
#endif

  // Head in direction dir.
  m_npc->SetDir(dir);
  // Fast - but decel?
  float speed = 100.f + Rnd(0.f, 50.f);
  m_npc->SetVel(GetVecFromAngleDegs(dir) * speed);
  m_npc->SetAnim("walk");
}
}

