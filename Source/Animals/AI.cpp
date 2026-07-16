#include <Timer.h>
#include "AI.h"
#include "Describe.h"
#include "Npc.h"

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

  if (m_maxTime > 0 && m_time > m_maxTime)
  {
std::cout << "AI for " << Describe(m_npc) << ": max time reached ("
  << m_maxTime << "s).\n";
    m_maxTime = 0; // set a new max time in OnActivated
//    m_npc->SetAI(nullptr); // so we can chose the same AI again but call OnActivated
    OnMaxTimeReached();
  }
}

void AI::OnMaxTimeReached()
{
  m_npc->DecideAI();
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
