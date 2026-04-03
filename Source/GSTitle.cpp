// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <ConfigFile.h>
#include <Directory.h>
#include <Game.h>
#include <GuiDecAnimation.h>
#include <MessageQueue.h>
#include <SoundManager.h>
#include "Consts.h"
#include "GSTitle.h"
#include "PlayWav.h"
#include "UserProfile.h"

namespace Amju
{
static void OnStart(GuiElement* elem)
{
  TheSoundManager::Instance()->StopSong(); // TODO play next song

  PlayWav(WAV_START_BUTTON); // This wav should blend nicely with the title music
}

GSTitle::GSTitle()
{
  m_guiFilename = "Gui/gs_title.txt";
}

void GSTitle::OnActive()
{
  GSBase::OnActive();

  // Start playing title music
  auto sm = TheSoundManager::Instance();
  sm->SetSongMaxVolume(0.1f); // for some reason it's deafening on Mac
  sm->PlaySong("Music/amt-title.it");

  // Set button commands
  GuiElement* startButton = GetElementByName(m_gui, "start-button");
  startButton->SetCommand(OnStart);
  startButton->SetHasFocus(true);
}
}


