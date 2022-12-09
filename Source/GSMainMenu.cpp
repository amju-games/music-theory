// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include <GuiButton.h>
#include <GuiDecAnimation.h>
#include <MessageQueue.h>
#include <ResourceManager.h>
#include <StringUtils.h>
#include "Course.h"
#include "GSAbout.h"
#include "GSMainMenu.h"
#include "GSTitle.h"
#include "GSTopicStart.h"
#include "Keys.h"
#include "UserProfile.h"

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

  // Get user config, so we know which topics have been unlocked.
  ConfigFile* config = TheUserProfile()->GetConfigForTopic(KEY_TOPICS);

  // Set button commands. For the buttons corresponding to Topics, the command
  //  sends us to that topic.
  // Get the course, loop over the topics in it.
  Course* course = GetCourse();
  Assert(course);
  int numTopics = course->GetNumTopics();

  GuiComposite* leafParent = dynamic_cast<GuiComposite*>(GetElementByName(m_gui, "scrolling-part-of-scene"));
  Assert(leafParent);

  for (int i = 0; i < numTopics; i++)
  {
    Topic* topic = course->GetTopic(i);

    bool unlocked = (i == 0) || config->Exists(KEY_TOPIC_UNLOCKED + ToString(i));

    // Create leaf button for topic
    const char* LEAF_FILE[2] = 
    {
      "Gui/topic-leaf-left.txt",
      "Gui/topic-leaf-right.txt"
    };
    PGuiElement topicRoot = LoadGui(LEAF_FILE[i % 2], false);
    Assert(topicRoot);
    // Add to composite in gui
    leafParent->AddChild(topicRoot);

    // Set wait time
    GuiDecAnimation* wait = dynamic_cast<GuiDecAnimation*>(GetElementByName(topicRoot, "wait"));
#ifdef _DEBUG
    // Temp: don't wait, for debug/dev
    wait->SetCycleTime(0.1f); // TODO TEMP TEST
#else
    wait->SetCycleTime(float)i * 1.2f); // TODO TEMP TEST
#endif

    // Set position
    topicRoot->SetLocalPos(Vec2f(0, (float)i * 0.15f)); // TODO TEMP TEST

    // Find the button corresponding to this topic
    GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(topicRoot, "topic-button"));
    Assert(button);
    button->SetCommand(new TopicCommand(i));

    // Set name for button
    IGuiText* text = dynamic_cast<IGuiText*>(GetElementByName(topicRoot, "topic-name-text"));
    Assert(text);
    text->SetText(topic->GetDisplayName());

#ifdef YES_LOCK_TOPICS
    if (unlocked)
    {
      // ?
      GuiDecAnimation* pulseAnim = dynamic_cast<GuiDecAnimation*>(GetElementByName(topicRoot, "pulse-leaf"));
      pulseAnim->SetEaseType(GuiDecAnimation::EaseType::EASE_TYPE_ONE);
    }
    else
    {
      button->SetIsEnabled(false);
      Colour c = button->GetButtonColour();
      c.m_a = 0.5f;
      button->SetButtonColour(c);
      // TODO text colour, but IGuiText has very limited interface
    }
#endif // YES_LOCK_TOPICS

  }

  // Back to title 
  GuiElement* title = GetElementByName(m_gui, "title-button");
  title->SetCommand(OnBackToTitle);

  GuiElement* about = GetElementByName(m_gui, "about-button");
  about->SetCommand(OnAbout);

  // TODO

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

