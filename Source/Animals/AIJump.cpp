#include <AmjuRand.h>
#include "AIJump.h"
#include "PFNpc.h"

namespace Amju
{
const char* AIJump::NAME = "jump";
const char* AIJump::GetName() const { return NAME; }

AIJump::AIJump()
{
  m_rank = -1000; // don't choose this one!
}

void AIJump::Update() 
{
  AIPetMoveBase::Update();
 
  const float y = m_npc->GetPos().y;
  if (y < 0)
  {
    OnActivated(); 
  }
}

void AIJump::OnActivated() 
{
  m_npc->SetAnim("fall");

  m_npc->SetVel({Rnd(-10.f, 10.f), 200.f + Rnd(0, 200.f), 0});
  m_npc->SetAcc({0, -800.f, 0}); // gravity

  m_npc->SetDir(Rnd(-30.f, 30.f));
}
}

