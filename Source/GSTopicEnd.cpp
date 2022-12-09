// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include <GuiText.h>
#include <MessageQueue.h>
#include "GSMainCorridor.h"
#include "GSTopicEnd.h"
#include "UserProfile.h"

namespace Amju
{
static void OnOK(GuiElement*)
{
  TheMessageQueue::Instance()->Add(new FuncMsg(GoTo<TheGSMainCorridor>, SecondsFromNow(1.0f)));
}

GSTopicEnd::GSTopicEnd()
{
  m_guiFilename = "Gui/gs_topic_end.txt";
}

void GSTopicEnd::OnActive()
{
  // This works well because it skips loading scene again in GSBase3d::OnActive
  GSBase::OnActive();

  // Set button commands
  GuiElement* elem = GetElementByName(m_gui, "ok-button");
  elem->SetCommand(OnOK);
  elem->SetHasFocus(true);

  GuiElement* shareButton = GetElementByName(m_gui, "share-button");
  Assert(shareButton);
  shareButton->SetCommand(Amju::OnShare);
  // TODO Set message and image
  
  auto profile = TheUserProfile();

  // Set score number text
  IGuiText* text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "num-text"));
  Assert(text);
  int score = profile->GetTopicScore();
  text->SetText(ToString(score));

  // Set comment about score etc
  text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "comment-text"));
  Assert(text);
  text->SetText(GenerateScoreComment());

  // Set topic display name
  text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "topic-name-text"));
  Assert(text);
  text->SetText(profile->GetCurrentTopicDisplayName());

  // Add to hints (TODO animate additions)
}

std::string GSTopicEnd::GenerateScoreComment()
{
  std::string res = "Awesome";
  return res;
}

}

