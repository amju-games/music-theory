// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include <GuiText.h>
#include "Course.h"
#include "GSMainCorridor.h"
#include "GSPages.h"
#include "GSTopicStart.h"
#include "PlayWav.h"
#include "Topic.h"

namespace Amju
{
static void OnBack(GuiElement*)
{
  PlayWav("bad1");

  // Back to main menu
  TheGame::Instance()->GetState()->GoBack();
}

static void OnStart(GuiElement*)
{
  PlayWav("good1");

  GSPages* gs = TheGSPages::Instance();
  int topicNum = TheGSTopicStart::Instance()->GetTopic();
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
  
  // Set topic display name
  // TODO Pass the course in
  Course* course = (Course*)TheResourceManager::Instance()->GetRes("Course/grade1.txt.course");
  Assert(course);
  IGuiText* text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "topic-name-text"));
  Assert(text);
  Topic* topic = course->GetTopic(m_topicNum);
  Assert(topic);
  text->SetText(topic->GetDisplayName());
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

