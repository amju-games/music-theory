// * Amjula Music School *
// (c) Copyright 2024 Juliet Colman

#include <EventPoller.h>
#include <File.h>
#include <LoadScene.h>
#include <LoadVec3.h>
#include "Consts.h"
#include "CorridorModeEnterTappable.h"
#include "GSMainCorridor.h"
#include "PlayWav.h"
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

  // Load "on-screen" GUI
  if (!f->GetDataLine(&m_screenGuiFilename))
  {
    f->ReportError("Expected screen GUI for tappable");
    return false;
  }
  ActivateGui();

  //// Load GUI we display in notebook (use includes to share as much as poss)
  //if (!f->GetDataLine(&m_notebookGuiFilename))
  //{
  //  f->ReportError("Expected notebook GUI for tappable");
  //  return false;
  //}

  return true;
}

PGuiElement Tappable::GetGui()
{
  Assert(m_gui);
  return m_gui;
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
  if (!m_screenGuiFilename.empty())
  {
    m_gui = LoadGui(m_screenGuiFilename);
  }
}

void Tappable::DeactivateGui()
{
  m_gui = nullptr;
}

void Tappable::DrawGui()
{
  if (m_gui)
  {
    m_gui->Draw();
  }
}

void Tappable::UpdateGui()
{
  if (m_gui)
  {
    m_gui->Update();
  }
}

void Tappable::OnTapped()
{
  // Default behaviour
  //TheGSMainCorridor::Instance()->SetMode(CorridorModeEnterTappable::ID);

  PlayWav(WAV_ENTER_TAPPABLE);
}
}
