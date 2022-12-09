// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Timer.h>
#include "GuiMusicScore.h"
#include "GSShowMusicScore.h"
#include "ScoreBuilder.h"

namespace Amju
{
void GSShowMusicScore::Update()
{
  Vec2f pos = m_score->GetLocalPos();
  float speed = 0.2f;
  float dt = TheTimer::Instance()->GetDt();
  pos.x -= dt * speed;
  m_score->SetLocalPos(pos);
}

void GSShowMusicScore::OnActive()
{
  GSBase::OnActive(); 
  m_gui = LoadGui("Gui/gs_score.txt");
  m_score = dynamic_cast<GuiMusicScore*>(GetElementByName(m_gui, "music-score"));

  ScoreBuilder sb;
  for (int i = 0; i < 25; i++)
  {
    sb.Add(1, 0, Note(i, i - 10, Note::SEMIBREVE));
  }
  sb.Write(*m_score);
}
}

