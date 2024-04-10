// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include "CorridorMode.h"

namespace Amju
{
void CorridorMode::Draw2d()
{
  if (m_gui)
  {
    m_gui->Draw();
  }
}

void CorridorMode::Update()
{
  if (m_gui)
  {
    m_gui->Update();
  }
}

void CorridorMode::OnActive()
{
  if (!m_guiFilename.empty())
  {
    m_gui = LoadGui(m_guiFilename);
  }
}

void CorridorMode::OnDeactive()
{
  m_gui = nullptr;
}
}
