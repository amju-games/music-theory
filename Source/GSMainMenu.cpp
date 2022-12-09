// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include <GuiButton.h>
#include <GuiDecAnimation.h>
#include <LoadScene.h>
#include <MessageQueue.h>
#include <ResourceManager.h>
#include <SceneGraph.h>
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
//  TheGSMainMenu::Instance()->HideButtons()->ScrollUp();
}

static void OnNewUser(GuiElement*)
{
//  TheGSMainMenu::Instance()->HideButtons()->ScrollDown();
}

static void OnAbout(GuiElement*)
{
//  TheGSMainMenu::Instance()->HideButtons()->ScrollDown();
  TheMessageQueue::Instance()->Add(new FuncMsg(GoTo<TheGSAbout>, SecondsFromNow(1.0f)));
}

static void OnBackToTitle(GuiElement*)
{
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
}

static void OnTopic(GuiElement*)
{
  // Topic button for all topics: the currently selected topic is the
  //  one we go to.
  TheGSMainMenu::Instance()->GoToTopic(0); // TODO TEMP TEST
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
  m_guiFilename = "Gui/gs_main_menu_corridor.txt";
  m_sceneFilename = "Scene/corridor-scene.txt";
}

void GSMainMenu::Load3dForTopics()
{
  SceneNode* root = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  SceneNode* camera = root->GetNodeByName("camera");
  Assert(camera);

  for (int i = 0; i < 2; i++)
  {
    PSceneNode node = LoadScene("Scene/corridor-one-door.txt");
    Assert(node);
    node->SetIsLit(true);

    // Translate this node
    Matrix m;
    //m.Scale(0.5f, 0.5f, 0.5f);
    // Camera is looking down the x axis, so we translate each piece of the 
    //  corridor in z... TODO make this translation automatically at
    //  right angles to view vec?
    m.Translate(Vec3f(0, 0, i * -300.0f)); // TODO TEMP TEST
    node->SetLocalTransform(m);

    camera->AddChild(node);
  }
}

void GSMainMenu::OnActive()
{
  // Load the GUI. The buttons here correspond to Topics in the Course, but
  //  we don't want to try to programatically add the buttons. So the GUI layout
  //  here is coupled to the Course definition.
  GSBase3d::OnActive();

  // Get user config, so we know which topics have been unlocked.
  ConfigFile* config = TheUserProfile()->GetConfigForTopic(KEY_TOPICS);

  // Just one topic button, which is fixed in the centre of the screen.
  // The scene scrolls left and right, but sticks so each door is under the 
  //  button.
  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "topic-button"));
  Assert(button);
  button->SetCommand(OnTopic); 

  // Set topic name - do this dynamically as we scroll
  IGuiText* text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "topic-name-text"));
  Assert(text);
  text->SetText("Topic name"); // topic->GetDisplayName());

  // Back to title 
  GuiElement* title = GetElementByName(m_gui, "title-button");
  title->SetCommand(OnBackToTitle);

  GuiElement* about = GetElementByName(m_gui, "about-button");
  about->SetCommand(OnAbout);

  Load3dForTopics();

//  // Set button commands. For the buttons corresponding to Topics, the command
//  //  sends us to that topic.
//  // Get the course, loop over the topics in it.
//  Course* course = GetCourse();
//  Assert(course);
//  int numTopics = course->GetNumTopics();
//
//  GuiComposite* leafParent = dynamic_cast<GuiComposite*>(GetElementByName(m_gui, "scrolling-part-of-scene"));
//  Assert(leafParent);
//
//  for (int i = 0; i < numTopics; i++)
//  {
//    Topic* topic = course->GetTopic(i);
//
//    bool unlocked = (i == 0) || config->Exists(KEY_TOPIC_UNLOCKED + ToString(i));
//
//    // Create button for topic
//    const char* LEAF_FILE[2] = 
//    {
//      "Gui/topic_door_1.txt",
//      // TODO more door types
//    };
//    PGuiElement topicRoot = LoadGui(LEAF_FILE[0], false);
//
//    Assert(topicRoot);
//    // Add to composite in gui
//    leafParent->AddChild(topicRoot);
//
//#ifdef YES_WAIT_FOR_TOPIC
//    // Set wait time: topic button becomes visible/active after this time
//    GuiDecAnimation* wait = dynamic_cast<GuiDecAnimation*>(GetElementByName(topicRoot, "wait"));
//#ifdef _DEBUG
//    // Temp: don't wait, for debug/dev
//    wait->SetCycleTime(0.1f); // TODO TEMP TEST
//#else
//    wait->SetCycleTime(float)i * 1.2f); // TODO TEMP TEST
//#endif
//#endif // YES_WAIT_FOR_TOPIC
//
//    // Set position of topic button and associated GUI
//    Vec2f pos(static_cast<float>(i) * 0.8f - 1.0f, 0); // position horizontally
//    topicRoot->SetLocalPos(pos);
//
//    // Find the button corresponding to this topic
//    GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "topic-button"));
//    Assert(button);
//    button->SetCommand(new TopicCommand(i));
//
//    // Set name for button
//    IGuiText* text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "topic-name-text"));
//    Assert(text);
//    text->SetText(topic->GetDisplayName());
//
//#ifdef YES_LOCK_TOPICS
//    if (unlocked)
//    {
//      // ?
//      GuiDecAnimation* pulseAnim = dynamic_cast<GuiDecAnimation*>(GetElementByName(topicRoot, "pulse-leaf"));
//      pulseAnim->SetEaseType(GuiDecAnimation::EaseType::EASE_TYPE_ONE);
//    }
//    else
//    {
//      button->SetIsEnabled(false);
//      Colour c = button->GetButtonColour();
//      c.m_a = 0.5f;
//      button->SetButtonColour(c);
//      // TODO text colour, but IGuiText has very limited interface
//    }
//#endif // YES_LOCK_TOPICS

//  }



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
  // Time delay so we get to see an animation, e.g. door opening
  TheMessageQueue::Instance()->Add(new FuncMsg(GoTo<TheGSTopicStart>, SecondsFromNow(1.0f)));

  HideButtons();

  // Start animation
//ScrollUp();
}

bool GSMainMenu::OnCursorEvent(const CursorEvent& ce)
{
  if (m_isDragging)
  {
    Vec2f pos = Vec2f(ce.x, ce.y);
    Vec2f dragDist = pos - m_touchDown;
    const float MIN_DRAG_DIST = 0.5f; // 1/4 of screen
    if (fabs(dragDist.x) > MIN_DRAG_DIST)
    {
      if (dragDist.x > 0)
      {
        std::cout << "Drag right!\n";
      }
      else
      {
        std::cout << "Drag left!\n";
      }
      m_touchDown = pos;
    }
  }
  return false;
}

bool GSMainMenu::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  m_isDragging = mbe.isDown;
  if (mbe.isDown)
  {
    m_touchDown = Vec2f(mbe.x, mbe.y);
  }
  return false;
}

}

