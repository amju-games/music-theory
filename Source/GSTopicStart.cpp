// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <Game.h>
#include <GuiText.h>
#include "ComposerList.h"
#include "Consts.h"
#include "Course.h"
#include "CourseManager.h"
#include "GSMainCorridor.h"
#include "GSPages.h"
#include "GSTopicEnd.h"
#include "GSTopicStart.h"
#include "GuiAvatar.h"
#include "LevelManager.h"
#include "NumUpdate.h"
#include "PlayWav.h"
#include "Topic.h"
#include "UserProfile.h"
#include "Wallpaper.h"

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
}

void GSTopicStart::OnActive()
{
  GSBase::OnActive();

  Wallpaper w;
  w.SetWallpaper(GetGui(), GetLevelManager().GetLevelNum());

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
//  int numHints = profile->GetHints(HintType::HINT_TYPE_HINT); // TODO
//  NumUpdate(m_gui, "hint-counter" /* TODO CONST */, numHints);

  // Set best so far %
  Course* course = GetCourseManager().GetCourse();
  Assert(course);
  Topic* topic = course->GetTopic(profile->GetCurrentTopic());
  Assert(topic);
  
  int best = profile->GetBestTopicScore(topic->GetId());
  text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "best-so-far-num"));
  Assert(text);
  text->SetText(ToString(best) + "%");

  // Set this best score in Topic End state, so we can do stuff if
  //  we get a new best.
  TheGSTopicEnd::Instance()->SetBestScore(best);

  // Set Composer avatar for this topic
  auto composer = GetComposerList().GetComposerForTopic(topic->GetId());
  GuiAvatar* avatar = dynamic_cast<GuiAvatar*>(GetElementByName(m_gui, "composer-avatar"));
  avatar->SetFromString(composer->GetAvatarStr());

  // Set composer name on nameplate.
  GuiButton* nameplate = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "composer-name-button"));
  Assert(nameplate);
  nameplate->SetText(composer->GetDisplayName());
}
}

