#include <AmjuRand.h>
#include "AIIdle.h"
#include "Npc.h"

namespace Amju
{
const char* AIIdle::NAME = "idle";

AIIdle::AIIdle()
{
  m_rank = 100.0f;
}

const char* AIIdle::GetName() const
{
  return NAME;
}

void AIIdle::OnActivated()
{
  AI::OnActivated();

  m_npc->SetAnim("stand");
  m_npc->SetVel(Vec3f(0, 0, 0));

  // Choose time until we re-decide
  m_maxTime = 2.f + Rnd(0.f, 2.0f);
}
}
