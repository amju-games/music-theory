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

bool GSBase::OnKeyEvent(const KeyEvent& ke)
{
#ifdef _DEBUG
  // Reload score
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR &&
    (ke.key == 'r' || ke.key == 'R'))
  {
    std::cout << "Reloading\n";
    OnActive(); // or factor out score loading
    return true;
  }
#endif
  return false;
}

}
