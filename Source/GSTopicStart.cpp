// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include "GSMainMenu.h"
#include "GSPages.h"
#include "GSTopicStart.h"

namespace Amju
{
static void OnBack(GuiElement*)
{
  // Back to main menu
  TheGame::Instance()->GetState()->GoBack();
}

static void OnStart(GuiElement*)
{
  GSPages* gs = TheGSPages::Instance();
  int topicNum = TheGSTopicStart::Instance()->GetTopic();
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

  // Set button commands
  GuiElement* elem = GetElementByName(m_gui, "cancel-button");
  elem->SetCommand(OnBack);

  elem = GetElementByName(m_gui, "start-button");
  elem->SetCommand(OnStart);
}

void GSTopicStart::SetTopic(int topicNum)
{
  m_topicNum = topicNum;
}

int GSTopicStart::GetTopic() const
{
  return m_topicNum;
}
}

