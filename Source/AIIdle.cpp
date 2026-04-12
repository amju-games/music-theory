#include "AIIdle.h"
#include "Npc.h"

namespace Amju
{
const char* AIIdle::NAME = "aiidle";

AIIdle::AIIdle()
{
  m_rank = 100.0f;
}

const char* AIIdle::GetName() const
{
  return NAME;
}

void AIIdle::Update()
{
  AI::Update();
  Assert(m_npc);

  // Wait for delay, then Decide
  if (m_time > 2.0f) // TODO
  {
    m_npc->DecideAI();
  }
}

void AIIdle::OnActivated()
{
  AI::OnActivated();

  m_npc->SetAnim("stand");
  m_npc->SetVel(Vec3f(0, 0, 0));
}
}
