// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#include <EventPoller.h>
#include <File.h>
#include <LoadScene.h>
#include <LoadVec3.h>
#include "Tappable.h"

namespace Amju
{
bool Tappable::Load(File* f)
{
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Expected tappable name");
    return false;
  }

  // 3D mesh
  m_node = LoadScene(f);
  
  // Camera eye/target when selected
  if (!LoadVec3(f, &m_camEye))
  {
    f->ReportError("Failed to load eye pos for camera node");
    return false;
  }

  if (!LoadVec3(f, &m_camTarget))
  {
    f->ReportError("Failed to load look at (target) pos for camera node");
    return false;
  }

  // Load "on-screen" GUI
  if (!f->GetDataLine(&m_screenGuiFilename))
  {
    f->ReportError("Expected screen GUI for tappable");
    return false;
  }

  // Load GUI we display in notebook (use includes to share as much as poss)
  if (!f->GetDataLine(&m_notebookGuiFilename))
  {
    f->ReportError("Expected notebook GUI for tappable");
    return false;
  }

  return true;
}

SceneNode* Tappable::GetSceneNode()
{
  return m_node;
}

const std::string& Tappable::GetName() const
{
  return m_name;
}

const Vec3f& Tappable::GetCameraEyePos() const
{
  return m_camEye;
}

const Vec3f& Tappable::GetCameraTargetPos() const
{
  return m_camTarget;
}

void Tappable::ActivateGui()
{
  //TheEventPoller::Instance()->AddListener(m_gui);
  //m_gui->SetVisible(true);

  m_gui = LoadGui(m_screenGuiFilename);
}

void Tappable::DrawGui()
{
  Assert(m_gui);
  m_gui->Draw();
}
}
