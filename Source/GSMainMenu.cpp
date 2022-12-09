// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include <GuiButton.h>
#include <MessageQueue.h>
#include <ResourceManager.h>
#include <StringUtils.h>
#include "Course.h"
#include "GSAbout.h"
#include "GSMainMenu.h"
#include "GSTitle.h"
#include "GSTopicStart.h"

namespace Amju
{
static void OnShare(GuiElement*)
{
  TheGSMainMenu::Instance()->HideButtons()->ScrollUp();
}

static void OnNewUser(GuiElement*)
{
  TheGSMainMenu::Instance()->HideButtons()->ScrollDown();
}

static void OnAbout(GuiElement*)
{
  TheGSMainMenu::Instance()->HideButtons()->ScrollDown();
  TheMessageQueue::Instance()->Add(new FuncMsg(GoTo<TheGSAbout>, SecondsFromNow(1.0f)));
}

static void OnBackToTitle(GuiElement*)
{
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
}

class TopicCommand : public GuiCommand
{
public:
  TopicCommand(int topicNum) : m_topicNum(topicNum) {}
  virtual bool Do() override
  {
    TheGSMainMenu::Instance()->GoToTopic(m_topicNum);
    return false; // no undo
  }

private:
  int m_topicNum = -1;
};

GSMainMenu::GSMainMenu()
{
  m_guiFilename = "Gui/gs_main_menu.txt";
}

void GSMainMenu::OnActive()
{
  // Load the GUI. The buttons here correspond to Topics in the Course, but
  //  we don't want to try to programatically add the buttons. So the GUI layout
  //  here is coupled to the Course definition.
  GSBase::OnActive();

  // Set button commands. For the buttons corresponding to Topics, the command
  //  sends us to that topic.
  // Get the course, loop over the topics in it.
  // A Course is a Resource.
  Course* course = GetCourse();
  Assert(course);
  int numTopics = course->GetNumTopics();
  for (int i = 0; i < numTopics; i++)
  {
    // Find the button corresponding to this topic, should be named "topic-button-<n>"
    GuiButton* elem = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "topic-button-" + ToString(i)));
    Assert(elem);
    elem->SetCommand(new TopicCommand(i));
  }

  // Back to title 
  GuiElement* title = GetElementByName(m_gui, "title-button");
  title->SetCommand(OnBackToTitle);

  GuiElement* about = GetElementByName(m_gui, "about-button");
  about->SetCommand(OnAbout);

  //GuiElement* share = GetElementByName(m_gui, "share-button");
  //share->SetCommand(OnShare);

  //GuiElement* newUser = GetElementByName(m_gui, "new-button");
  //newUser->SetCommand(OnNewUser);
}

void GSMainMenu::GoToTopic(int topic)
{
  // Go the the topic start state.
  GSTopicStart* gs = TheGSTopicStart::Instance();
  gs->SetTopic(topic);
  gs->SetPrevState(this);
  TheMessageQueue::Instance()->Add(new FuncMsg(GoTo<TheGSTopicStart>, SecondsFromNow(1.0f)));

  HideButtons();

  // Start animation
  ScrollUp();
}

}

