// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <iostream>
#include <CommandLineArgs.h>
#include <MessageQueue.h>
#include <SoundManager.h>
#include "Consts.h"
#include "GSHero.h"

namespace Amju
{
GSHero::GSHero()
{
  m_guiFilename = "Gui/gs_hero.txt";
}

void GSHero::Update()
{
  GSBase::Update();
}

void GSHero::Start()
{
std::cout << "START!!\n";

  // Start score scrolling, and set backing track playing at the same time.
  // The score and the song have to be in perfect sync.
  // TODO Is there a way to sync the song?
  m_scoreAnim->SetIsPaused(false);

  auto sm = TheSoundManager::Instance();
  sm->SetSongMaxVolume(0.1f); // for some reason it's deafening on Mac
  sm->PlaySong("Music/amt1.it");
}

void GSHero::OnMusicKbEvent(const MusicKbEvent& e) 
{
std::cout << "Got keyboard event\n";
  // TODO
//  m_scrollScore->GradeEvent(e);
}

void GSHero::OnActive() 
{
  GSBase::OnActive();  

  auto* elem = GetElementByName(m_gui, "the-score");
  if (!elem)
  {
    std::cout << "GUI Score element called \"the-score\" not found.\n";
    Assert(0); 
  }
 
  m_scrollScore = dynamic_cast<GuiScrollScore*>(elem);
  if (!m_scrollScore)
  {
    std::cout << "GUI Score element called \"the-score\" was found but it's the wrong type.\n";
    Assert(0); 
  }

  // Find the animator parent too.
  elem = m_scrollScore->GetParent();
  Assert(elem);
  m_scoreAnim = dynamic_cast<GuiDecAnimation*>(elem);
  if (!m_scoreAnim)
  {
    std::cout << "Score does not have an animator parent.\n";
    Assert(0);
  }

  // Set the animation time from the score meta data
  auto songLength = m_scrollScore->GetSongLengthSeconds();
  if (songLength)
  {
    m_scoreAnim->SetCycleTime(*songLength);
  }

  // Pause animation until we are ready to start, right?
  m_scoreAnim->SetIsPaused(true);

std::cout << "Paused...\n";

  // Start after a short pause, TODO TEMP TEST
  TheMessageQueue::Instance()->Add(new FuncMsg(
    [](){ TheGSHero::Instance()->Start(); }, SecondsFromNow(3.f)));
}
}
