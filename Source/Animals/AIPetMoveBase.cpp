#include <Timer.h>
#include "AIPetMoveBase.h"
#include "AngleVec.h"
#include "PFNpc.h"

namespace Amju
{
void AIPetMoveBase::StayOnScreen()
{
  // If we are heading off screen, turn around.
  const float TURN_VEL = 90.f; // ..with this turn vel

  const float dt = TheTimer::Instance()->GetDt();

  // Start turning to avoid side of screen when we are this far from
  //  the edge.
  const float EDGE = PFNpc::OFF_SCREEN_X * .8f;
 
  const float x = m_npc->GetPos().x;
  const float vx = m_npc->GetVel().x;

  if ((x < -EDGE && vx < 0) || (x > EDGE && vx > 0)) 
  {
    // Turn around
    float dir = m_npc->GetDir();
    dir += dt * TURN_VEL; 
    m_npc->SetDir(dir);
    m_npc->SetVel(GetVecFromAngleDegs(dir));
  }
}
}

