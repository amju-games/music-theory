#include <Timer.h>
#include "AI.h"

namespace Amju
{
void AI::SetNpc(Npc* npc)
{
  m_npc = npc;
  Assert((GameObject*)m_npc != m_target); // why the same?
}

void AI::SetTarget(GameObject* target)
{
  m_target = target;
  Assert((GameObject*)m_npc != m_target); // why the same?
}

void AI::Update()
{
  m_time += TheTimer::Instance()->GetDt();
}

void AI::OnActivated()
{
  m_time = 0;
}

float AI::GetRank()
{
  return m_rank;
}
}
