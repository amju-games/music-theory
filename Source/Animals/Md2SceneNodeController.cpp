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
  Assert(m_sceneNode);
  int anim = m_sceneNode->GetMd2()->GetAnimationFromName(animName);
  if (anim != m_anim)
  {
std::cout << "Setting anim: " << animName << "\n";
    m_anim = anim;
    m_sceneNode->SetAnim(animName);
  }
}
}

