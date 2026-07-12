#include <iostream>
#include <AmjuAssert.h>
#include "AIController.h"
#include "Npc.h"

#define AI_DEBUG

namespace Amju
{
#ifdef AI_DEBUG
std::string Describe(const AIController* aic) 
{
  auto go = dynamic_cast<const GameObject*>(aic);
  Assert(go);
  return std::string(go->GetTypeName()) + " ID: " + std::to_string(go->GetId());
}
#endif

AIController::AIController()
{
  m_ai = 0;
}
  
AI* AIController::GetAI(const std::string& aiName)
{
  auto it = m_ais.find(aiName);
  Assert(it != m_ais.end());
  return it->second;
}

void AIController::DecideAI()
{
  typedef std::pair<float, AI*> RankAI;
  RankAI best(0.0f, (AI*)0);

  // Rank each behaviour, choose best
  for (AIs::iterator it = m_ais.begin(); it != m_ais.end(); ++it)
  {
    AI* ai = it->second;
    float rank = ai->GetRank();
#ifdef AI_DEBUG
    std::cout << Describe(this) << ": rank for " 
      << ai->GetName() << ": " << rank << "\n";   
#endif

    if (rank >= best.first)
    {
      best.first = rank;
      best.second = ai;
    }
  }
#ifdef AI_DEBUG
  std::cout << Describe(this) << " choosing AI: " << best.second->GetName() 
    << " rank: " << best.first << "\n";
#endif
  SetAI(best.second);
}

void AIController::AddAI(AI* ai)
{
  m_ais[ai->GetName()] = ai;
  ai->SetNpc(dynamic_cast<Npc*>(this));
}

void AIController::SetAI(AI* ai)
{
  if (m_ai == ai)
  {
    return;
  }

  if (m_ai)
  {
    m_ai->OnDeactivated();
  }
  m_ai = ai;
#ifdef AI_DEBUG
  std::cout << Describe(this) << " changing AI to " << m_ai->GetName() << "\n";
#endif
  m_ai->OnActivated();
}

void AIController::SetAI(const std::string& aiName)
{
  Assert(m_ais.find(aiName) != m_ais.end());
  SetAI(m_ais[aiName]);
}

void AIController::UpdateAI()
{
/*
  if (IsDead())
  {
    return;
  }
*/

/*
  // We might actually want this for birds.
  if (!IsOnFloor())
  {
    // Falling
    SetAI(AIFalling::NAME); 
  }
*/
  if (m_ai)
  {
    m_ai->Update();
  }
}

void AIController::OnAnimFinished()
{
  if (m_ai)
  {
    m_ai->OnAnimFinished();
  }
}

void AIController::OnAnimFreeze()
{
  if (m_ai)
  {
    m_ai->OnAnimFreeze();
  }
}

void AIController::OnAnimRepeat()
{
  if (m_ai)
  {
    m_ai->OnAnimRepeat();
  }
}
}

