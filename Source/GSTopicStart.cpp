// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include <GuiText.h>
#include "Consts.h"
#include "Course.h"
#include "GSMainCorridor.h"
#include "GSPages.h"
#include "GSTopicStart.h"
#include "NumUpdate.h"
#include "PlayWav.h"
#include "Topic.h"
#include "UserProfile.h"

namespace Amju
{
static void OnBack(GuiElement*)
{
  PlayWav(WAV_CANCEL);

  // Back to main menu
  TheGame::Instance()->GetState()->GoBack();
}

static void OnStart(GuiElement*)
{
  PlayWav(WAV_START_TOPIC);

  GSPages* gs = TheGSPages::Instance();
  int topicNum = TheUserProfile()->GetCurrentTopic();
  gs->StartTopic(topicNum);
  TheGame::Instance()->SetCurrentState(gs);
}

GSTopicStart::GSTopicStart()
{
  m_guiFilename = "Gui/gs_topic_start.txt";
  m_sceneFilename = "Scene/topic_start_scene.txt";
}

void GSTopicStart::OnActive()
{
  GSBase3d::OnActive();

  // Set button commands
  GuiElement* elem = GetElementByName(m_gui, "cancel-button");
  elem->SetCommand(OnBack);

  elem = GetElementByName(m_gui, "start-button");
  elem->SetCommand(OnStart);
  elem->SetHasFocus(true);
  
  // Set topic display name
  IGuiText* text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "topic-name-text"));
  Assert(text);
  text->SetText(TheUserProfile()->GetCurrentTopicDisplayName());

  auto profile = TheUserProfile();
  int numHints = profile->GetHints(HintType::HINT_TYPE_HINT); // TODO
  NumUpdate(m_gui, "hint-counter" /* TODO CONST */, numHints);
}
}

