// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <ConfigFile.h>
#include <Directory.h>
#include <Game.h>
#include <GuiDecAnimation.h>
#include <MessageQueue.h>
#include "Consts.h"
#include "GSCopyAssets.h" // for CopyFromGlueFile, DEPRECATED
#include "GSFirstUser.h" // TODO Use it or lose it
#include "GSMainCorridor.h"
#include "GSTitle.h"
#include "PlayWav.h"
#include "UserProfile.h"

namespace Amju
{
static void OnStart(GuiElement* elem)
{
  PlayWav(WAV_START_BUTTON);

  // Is there a user yet?
  ConfigFile* cf = TheUserProfile()->GetConfigForTopic(KEY_GENERAL);
  Assert(cf);
  if (true)//cf->Exists(KEY_USER_NAME))
  {
//    TheGSTitle::Instance()->HideButtons()->ScrollUp();
    TheMessageQueue::Instance()->Add(new FuncMsg(GoTo<TheGSMainCorridor>, SecondsFromNow(1.5f)));
  }
  else
  {
    // No user profile yet. Go to first user state.
//    TheGSTitle::Instance()->HideButtons()->ScrollUp();
    TheMessageQueue::Instance()->Add(new FuncMsg(GoTo<TheGSFirstUser>, SecondsFromNow(1.5f)));
  }
}

GSTitle::GSTitle()
{
  m_guiFilename = "Gui/gs_title.txt";
  m_sceneFilename = "Scene/title-scene.txt";
}

void GSTitle::OnActive()
{
  GSBase3d::OnActive();

  // Copy ROConfig from glue file into save dir. This is almost pointless for this
  //  game, so as an optimisation, get rid of this step
  CopyFromGlueFile(GetSaveDir(APPNAME));
  
  // Set button commands
  GuiElement* startButton = GetElementByName(m_gui, "start-button");
  startButton->SetCommand(OnStart);
  startButton->SetHasFocus(true);
}

}


