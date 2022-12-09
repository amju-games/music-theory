// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GameState.h>
#include "Page.h"

namespace Amju
{
void Page::OnActive()
{
  // TODO append orientation to gui name
  m_gui = LoadGui("Gui/page_" + m_guiName + ".txt");

  // Adjust position so it sits under the top bar
  m_gui->SetLocalPos(Vec2f(0, -0.2f));
}

void Page::Draw()
{
  m_gui->Draw();
}

void Page::Update()
{
  m_gui->Update();
}

void Page::SetGameState(GameState* gs)
{
  m_gs = gs;
}

}

