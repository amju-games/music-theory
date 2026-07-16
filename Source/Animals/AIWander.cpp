#include <AmjuRand.h>
#include "AIWander.h"
#include "AngleVec.h"
#include "PFNpc.h"

namespace Amju
{
const char* AIWander::NAME = "wander";

const char* AIWander::GetName() const { return NAME; }

void AIWander::Update() 
{
  AI::Update();

  const float EDGE = 220.f;
  const float x = m_npc->GetPos().x;
  if (x < -EDGE || x > EDGE)
  {
    // Stop near edge
    m_npc->SetVel({});
  }
}

void AIWander::OnActivated() 
{
  AI::OnActivated();

  // Decide on direction to go, and how long for.
  m_maxTime = 1.5f + Rnd(0, 1.f);

  // Prefer to not wander off screen. So if we are near the edge,
  //  restrict the direction we can go in.
  float dir = 0;
  float x = m_npc->GetPos().x;
  const float EDGE = 100.f; 
  if (x < -EDGE)
  {
    dir = 90.f; //Rnd(0.f, 180.f);
  }
  else if (x > EDGE)
  {
    dir = -90.f; //Rnd(-180.f, 0.f);
  }
  else
  {
    dir = Rnd(-180.f, 180.f);
  }
std::cout << "Wander dir: " << dir << " degs\n";

  // Head in direction dir.
  m_npc->SetDir(dir);
  float speed = 50.f + Rnd(0.f, 50.f);
  m_npc->SetVel(GetVecFromAngleDegs(dir) * speed);
  m_npc->SetAnim("walk");
  // TODO Conceptually move around circle in Update over time.  
}

float AIWander::GetRank()
{
  // We can't calculate this in OnActivated, because unless we are
  //  activated, we will never calculate a new value.

  // How likely are we to keep wandering?
  m_rank = Rnd(0, 200);
  return m_rank;
}
}

