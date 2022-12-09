// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include <ResourceManager.h>
#include <StringUtils.h>
#include "Course.h"
#include "GSMainMenu.h"
#include "GSTopicStart.h"

namespace Amju
{
class TopicCommand : public GuiCommand
{
public:
  TopicCommand(int topicNum) : m_topicNum(topicNum) {}
  virtual bool Do() override
  {
    // Go the the topic start state.
    GSTopicStart* gs = TheGSTopicStart::Instance();
    gs->SetTopic(m_topicNum);
    gs->SetPrevState(TheGSMainMenu::Instance());
    TheGame::Instance()->SetCurrentState(gs);

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
  Course* course = (Course*)TheResourceManager::Instance()->GetRes("Course/grade1.txt.course");
  Assert(course);
  int numTopics = course->GetNumTopics();
  for (int i = 0; i < numTopics; i++)
  {
    // Find the button corresponding to this topic, should be named "topic-button-<n>"
    GuiElement* elem = GetElementByName(m_gui, "topic-button-" + ToString(i));
    Assert(elem);
    elem->SetCommand(new TopicCommand(i));
  }
}

}

