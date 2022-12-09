// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "Page.h"

namespace Amju
{
void Page::OnActive()
{
  GSBase::OnActive();
  // TODO append orientation to gui name
  m_gui = LoadGui("Gui/" + m_guiName + ".txt");

}
}

