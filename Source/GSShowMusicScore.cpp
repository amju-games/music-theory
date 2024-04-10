// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <StringUtils.h>
#include <Timer.h>
#include "GuiMusicScore.h"
#include "GSShowMusicScore.h"
#include "ScoreBuilder.h"

namespace Amju
{
GSShowMusicScore::GSShowMusicScore()
{
  m_guiFilename = "Gui/gs_score.txt";
}

void GSShowMusicScore::Update()
{
  GSBase::Update();

  //Vec2f pos = m_score->GetLocalPos();
  //float speed = 0.2f;
  //float dt = TheTimer::Instance()->GetDt();
  //pos.x -= dt * speed;
  //m_score->SetLocalPos(pos);
}

void GSShowMusicScore::OnActive()
{
  GSBase::OnActive(); 
  m_gui = LoadGui("Gui/gs_score.txt");

//  // Dots
//  for (int i = 0; i < 7; i++)
//  {
//    m_dots[i] = GetElementByName(m_gui, "kb-dot" + ToString(i + 1));
//    // Need colour decorator, I'm a bit surprised we can't set image colour :#
//    m_dots[i]->Set
//  }

  m_score = dynamic_cast<GuiMusicScore*>(GetElementByName(m_gui, "music-score"));

  //ScoreBuilder sb;
  //for (int i = 0; i < 25; i++)
  //{
  //  int bar = 1;
  //  int pitch = i;
  //  int x = i * 10;
  //  int y = i - 10;
  //  int duration = Note::SEMIBREVE;
  //  sb.Add(Note(pitch, x, y, duration, bar));
  //}
  //sb.Write(*m_score);
}

void GSShowMusicScore::SetOctaveDot()
{
  // Set dot to show current octave. Dots are grey, except for the current octave, which
  //  is white.
}

}

