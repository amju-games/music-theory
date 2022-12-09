// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "GSBase.h"

namespace Amju
{
void GSBase::Draw2d() 
{
  if (m_gui)
  { 
    m_gui->Draw();
  }
}

void GSBase::OnActive() 
{
  GameState::OnActive();
}

void GSBase::OnDeactive()
{
  GameState::OnDeactive();
}

}

