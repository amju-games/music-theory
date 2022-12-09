// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "Page.h"

namespace Amju
{
void Page::OnActive()
{
  // TODO append orientation to gui name
  m_gui = LoadGui("Gui/page_" + m_guiName + ".txt");

}

void Page::Draw()
{
  m_gui->Draw();
}

void Page::Update()
{
  m_gui->Update();
}
}

