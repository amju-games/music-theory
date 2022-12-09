// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "GSShowLineDrawing.h"

namespace Amju
{
void GSShowLineDrawing::OnActive()
{
  GSBase::OnActive(); 
  m_gui = LoadGui("Gui/gs_line_drawing.txt");
}

}

