// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "GSShowLineDrawing.h"

namespace Amju
{
void GSShowLineDrawing::Update()
{
  m_gui->Update();
}

void GSShowLineDrawing::OnActive()
{
  GSBase::OnActive(); 
  m_gui = LoadGui("c:/Users/Jason/projects/music-theory/Assets/Gui/gs_line_drawing.txt");
}

bool GSShowLineDrawing::OnKeyEvent(const KeyEvent& ke)
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

