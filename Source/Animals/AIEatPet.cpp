#include <DegRad.h>
#include "AIEatPet.h"
#include "Dino.h"
#include "PlayWav.h"

//#define AI_DEBUG

namespace Amju
{
const char* AIEatPet::NAME = "eat";

AIEatPet::AIEatPet()
{
  // Low rank for Decide(), but is set directly if we collide with a pet
  m_rank = -10;
}

const char* AIEatPet::GetName() const
{
  return NAME;
}
  
void AIEatPet::OnActivated() 
{
  AI::OnActivated();

  Assert(m_npc);
  Assert(m_target);

  m_npc->SetAnim("eat");

  // Target 'is being eaten' state; fully delete once eaten.
  auto targetNpc = dynamic_cast<PFNpc*>(m_target);
  Assert(targetNpc);
  // The target of the pet we are eating is this eater.
  auto eatenAI = targetNpc->GetAI("eaten");
  Assert(eatenAI);
  eatenAI->SetTarget(m_npc);
  targetNpc->SetAI(eatenAI); 

  // Stop dino moving but allow to fall to ground
  auto vel = m_npc->GetVel();
  vel.x = 0;
  vel.z = 0;
  m_npc->SetVel(vel); 

  PlayWav("goopy");

  /*
  // TODO All pets within range flee
  Pets pets;
  GetPets(&pets);
  for (Pets::iterator it = pets.begin(); it != pets.end(); ++it)
  {
    Pet* pet = *it;
    if (!pet->IsDead())
    {
      // TODO And within range
      AI* flee = pet->GetAI(AIFlee::NAME);
      flee->SetTarget(m_npc);
      pet->SetAI(flee);
    }
  }
  */
}

void AIEatPet::Update()
{
  AI::Update();

  Assert(m_npc);
  Assert(m_target);

  // Stop falling when we hit the ground!
  auto pos = m_npc->GetPos();
  if (pos.y < 0)
  {
    pos.y = 0;
    m_npc->SetPos(pos);
    m_npc->SetVel({});
  } 
}

void AIEatPet::OnAnimFinished()
{
  m_npc->SetAI("hide");
  PlayWav("burp");
}
}

