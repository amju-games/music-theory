#include <AmjuAssert.h>
#include "AIFalling.h"
#include "BlinkSceneNode.h"
#include "Md2SceneNode.h"
#include "Npc.h"

//#define AI_DEBUG

namespace Amju
{
AI* Npc::GetAI(const char* aiName)
{
  auto it = m_ais.find(aiName);
  Assert(it != m_ais.end());
  return it->second;
}

void Npc::DecideAI()
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
  std::cout << GetTypeName() << " choosing AI: " << best.second->GetName() 
    << " rank: " << best.first << "\n";
#endif
  SetAI(best.second);
}

void Npc::AddAI(AI* ai)
{
  m_ais[ai->GetName()] = ai;
  ai->SetNpc(this);
}

void Npc::SetAI(AI* ai)
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
  std::cout << GetTypeName() << " changing AI to " << m_ai->GetName() << "\n";
#endif
  m_ai->OnActivated();
}

void Npc::Update()
{
  GameObject::Update();

  if (m_ai)
  {
    m_ai->Update();
  }
}

void Npc::SetAnim(const std::string& animName)
{
  auto a = dynamic_cast<Md2SceneNode*>(GetSceneNode());
  Assert(a);
  int anim = a->GetMd2()->GetAnimationFromName(animName);
  m_anim = anim;
  a->SetAnim(animName);
}

void Npc::CreateSceneNode(PSceneNode parent)
{
  // All our NPCs in this game are blinking MD2 characters.
  auto sn = new BlinkSceneNode;
  sn->LoadMd2("md2/pz-bird.md2"); // TODO TEMP TEST
  sn->LoadTextures("Image/bird1.png", "Image/bird1a.png"); // TODO TEMP TEST
  m_sceneNode = sn;
  parent->AddChild(sn);
}

/*
void Npc::OnAnimFinished()
{
  if (m_ai)
  {
    m_ai->OnAnimFinished();
  }
}

void Npc::OnAnimFreeze()
{
  if (m_ai)
  {
    m_ai->OnAnimFreeze();
  }
}

void Npc::OnAnimRepeat()
{
  if (m_ai)
  {
    m_ai->OnAnimRepeat();
  }
}
*/
}
