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

void AIFlee::Update() 
{
  AI::Update();

  // Turn around when we are going off screen is same code as Wander.
  // TODO Common base class for moving AIs.

  // If we are heading off screen, turn around.
  const float TURN_VEL = 90.f;
  const float dt = TheTimer::Instance()->GetDt();

  const float EDGE = 200.f;
  const float x = m_npc->GetPos().x;
  const float vx = m_npc->GetVel().x;

  if ((x < -EDGE && vx < 0) || (x > EDGE && vx > 0))
  {
    // We are moving towards the edge of the screen: Turn around
    float dir = m_npc->GetDir();
    dir += dt * TURN_VEL; 
    m_npc->SetDir(dir);
    m_npc->SetVel(GetVecFromAngleDegs(dir));
  }
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
  const float EDGE = 120.f; 
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

