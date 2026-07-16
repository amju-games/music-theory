#include "AIEaten.h"
#include "PFNpc.h"

namespace Amju
{
const char* AIEaten::NAME = "eaten";

const char* AIEaten::GetName() const { return NAME; }

void AIEaten::OnActivated() 
{
  m_npc->SetAnim("eaten");

  // V quickly turn to face dino.
  // Target is the dino eating us: turn to face in direction of dino.
  auto vec = m_npc->GetPos() - m_target->GetPos();
  // Turn to face target, use convenience function wrapping atan2
  m_npc->SetDir(GetAngleDegsFromVec(vec)); 

  // Calc position of pet to match up with dino's mouth
  const float DIST_FROM_DINO_ORG = 5.f;
  auto desiredPos = m_target->GetPos() + Normalise(vec) * DIST_FROM_DINO_ORG;
  m_npc->SetPos(desiredPos);
  // This would work best if the origin for the eaten anim matches the
  //  origin of the dino. Also, the anims aren't great.
}

void AIEaten::OnAnimFreeze() 
{
  m_npc->SetVisible(false);
  // Go to do nothing state?
}
}

