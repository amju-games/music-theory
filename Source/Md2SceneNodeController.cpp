#include <iostream>
#include <AmjuAssert.h>
#include <DegRad.h>
#include <Timer.h>
#include "Md2SceneNodeController.h"

namespace Amju
{
void Md2SceneNodeController::UpdateMd2(const Vec3f& pos)
{
  float dt = TheTimer::Instance()->GetDt();

  Matrix mat;
  mat.RotateY(DegToRad(m_dirCurrent));
  mat.TranslateKeepRotation(pos);
  GetSceneNode()->SetLocalTransform(mat);

  static const float ROT_SPEED = 10.0f; // TODO config or param
  float angleDiff = m_dir - m_dirCurrent;
  
  // Rotate to face m_dir, taking the shortest route (CW or CCW)
  if (fabs(angleDiff) < 2.0f) // Hmm but why?
  {
    m_dirCurrent = m_dir;
  }
  else
  {
    if (angleDiff < -180.0f)
    {   
      m_dirCurrent -= 360.0f;
    }   
    else if (angleDiff > 180.0f)
    {   
      m_dirCurrent += 360.0f;
    }   

    angleDiff = m_dir - m_dirCurrent;
    if (m_dirCurrent > m_dir)
    {   
      m_dirCurrent -= ROT_SPEED * dt * fabs(angleDiff);
    }   
    else if (m_dirCurrent < m_dir)
    {
      m_dirCurrent += ROT_SPEED * dt * fabs(angleDiff);
    }
  }
}

void Md2SceneNodeController::SetAnim(const std::string& animName)
{
std::cout << "Setting anim: " << animName << "\n";

  auto a = dynamic_cast<Md2SceneNode*>(GetSceneNode());
  Assert(a);
  int anim = a->GetMd2()->GetAnimationFromName(animName);
  m_anim = anim;
  a->SetAnim(animName);
}

void Md2SceneNodeController::CreateSceneNode(PSceneNode parent)
{
  // This is an example. We want to load from a scene file, because we
  //  want Md2 scene node * and shadow *.

/*
  // All our NPCs in this game are blinking MD2 characters.
  auto sn = new BlinkSceneNode;
  sn->LoadMd2("md2/pz-bird.md2"); // TODO TEMP TEST
  sn->LoadTextures("Image/bird1.png", "Image/bird1a.png"); // TODO TEMP TEST
  m_sceneNode = sn;
  parent->AddChild(sn);
*/
}
}

