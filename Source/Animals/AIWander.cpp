#include <AmjuRand.h>
#include <Timer.h>
#include "AIWander.h"
#include "AngleVec.h"
#include "PFNpc.h"

namespace Amju
{
const char* AIWander::NAME = "wander";

AIWander::AIWander()
{
  // After initial wait period, the first thing we do is Wander
  //  on to the screen.
  m_rank = 1000.f;
}

const char* AIWander::GetName() const { return NAME; }

void AIWander::Update() 
{
  AI::Update();
  StayOnScreen();
}

void AIWander::OnActivated() 
{
  AI::OnActivated();

  // Reset rank so we yield to another behaviour. But in GetRank
  //  we increase rank each time so it becomes more likely we wander again.
  m_rank = 0;

  // Decide on direction to go, and how long for.
  m_maxTime = 1.5f + Rnd(0, 1.f);

  // Prefer to not wander off screen. So if we are near the edge,
  //  restrict the direction we can go in.
  float dir = 0;
  float x = m_npc->GetPos().x;
  const float EDGE = PFNpc::OFF_SCREEN_X * .5f;
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
    // Face left or right, with a bit of variety, but we don't want to 
    //  be too perpendicular to the camera.
    dir = Rnd(40.f, 80.f);
    if (RandomInt(2) == 0) dir = -dir;
  }
#ifdef AI_DEBUG
std::cout << "Wander dir: " << dir << " degs\n";
#endif

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
  // Add to the rank each time we decide. So it becomes more and more
  //  likely that we Wander. Then when we do, reset the rank.
  m_rank += Rnd(0, 30.f);
  return m_rank;
}
}

