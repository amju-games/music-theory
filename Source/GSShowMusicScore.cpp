// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "GSShowMusicScore.h"

namespace Amju
{

void GSShowMusicScore::Update()
{
}

void GSShowMusicScore::OnActive()
{
  GSBase::OnActive(); 
  m_gui = LoadGui("c:/Users/Jason/projects/music-theory/Assets/Gui/gs_score.txt");
}

}

