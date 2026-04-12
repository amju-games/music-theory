#include "AIFalling.h"
#include "Npc.h"

namespace Amju
{
const char* AIFalling::NAME = "ai-falling";

AIFalling::AIFalling()
{
  m_rank = -9999999.9f;
}

const char* AIFalling::GetName() const
{
  return NAME;
}

void AIFalling::Update()
{
  AI::Update();
  m_npc->SetAnim("stand"); // TODO fall
  // Check for floor - go to dead state when we hit the floor.
/*
  if (m_npc->IsOnFloor())
  {
    // Landed!
    m_npc->DecideAI();
  }
*/
}
}
