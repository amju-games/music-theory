// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <AmjuGL.h>
#include <ConfigFile.h>
#include <Game.h>
#include <GuiButton.h>
#include <GuiDecAnimation.h>
#include <GuiDecColour.h>
#include <GuiText.h>
#include <Localise.h>
#include <MessageQueue.h>

#include "Consts.h"
#include "Course.h"
#include "GSMainCorridor.h"
#include "GSPages.h"
#include "GSPause.h"
#include "GSTopicStart.h"
#include "GSTopicEnd.h"
#include "GuiLineDrawing.h"
#include "Hints.h"
#include "LurkMsg.h"
#include "Md2SceneNode.h" // TODO promote to Amjulib
#include "MySceneGraph.h"
#include "PlayWav.h"
#include "PrintGui.h"
#include "Topic.h"
#include "UserProfile.h"

namespace Amju
{
static void OnQuitConfirmOK(GuiElement*)
{
  TheGame::Instance()->SetCurrentState(TheGSMainCorridor::Instance());
}

static void OnQuitConfirmCancel(GuiElement*)
{
  TheGSPages::Instance()->OnQuitConfirmCancel();
}
  
static void OnPause(GuiElement*)
{
  TheGSPages::Instance()->OnPause();
}

// Called from timed FuncMessage
static void SpeechBubbleOK()
{
  GSPages* pages = TheGSPages::Instance();
  // Sanity check: this only makes sense if GSPages is active
  if (TheGame::Instance()->GetState() == pages)
  {
    pages->OnSpeechBubbleOK();
  }
}

static void OnSpeechBubbleOK(GuiElement*)
{
  SpeechBubbleOK();
}

GSPages::GSPages()
{
  m_guiFilename = "Gui/gs_pages_landscape.txt";
  m_sceneFilename = "Scene/room1-scene.txt";
}

void GSPages::StartTopic(int topicNum)
{
  m_numPagesShown = 0;
  m_numCorrectThisSession = 0;
  m_numIncorrectThisSession = 0;

  // TODO Get this from Topic. 
  m_maxNumPagesThisSession = 10;
}

void GSPages::ShowHints()
{
  IGuiText* hintCounter = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "hint-counter"));
  Assert(hintCounter);
  int hints = Hints::Get();
  hintCounter->SetText(ToString(hints));

  // Disable button if no hints available
  Assert(m_page);
  bool enabled = (hints > 0 && m_page->CanGetHint());
  SetButtonEnabled("hint-button", enabled);
}

void GSPages::ReloadGui()
{
  m_gui = LoadGui(m_guiFilename);

  GuiElement* elem = GetElementByName(m_gui, "pause-button");
  Assert(elem);
  elem->SetCommand(Amju::OnPause);

  elem = GetElementByName(m_gui, "quit-confirm-ok");
  Assert(elem);
  elem->SetCommand(Amju::OnQuitConfirmOK);

  elem = GetElementByName(m_gui, "quit-confirm-cancel");
  Assert(elem);
  elem->SetCommand(Amju::OnQuitConfirmCancel);

  elem = GetElementByName(m_gui, "speech-bubble-ok-button");
  Assert(elem);
  elem->SetCommand(Amju::OnSpeechBubbleOK);

  // Hide speech bubble initially
  GuiElement* speechBubble = GetElementByName(m_gui, "speech-bubble");
  speechBubble->SetVisible(false);
}

void GSPages::Reload3d()
{
  GSBase3d::Reload3d();

  SceneNode* root = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);

  // TEST
  // TODO Use Avatar system to make a 'teacher' avatar
  Md2SceneNode* md2node = new Md2SceneNode;
  md2node->LoadMd2("md2/av1.md2");
  //SceneNodeMaterial* dinoMaterial = new SceneNodeMaterial;
  //PTexture dinoTex = (Texture*)TheResourceManager::Instance()->GetRes("md2/skin.png");
  //dinoMaterial->SetTexture(dinoTex);
  //md2node->SetMaterial(dinoMaterial);
  md2node->SetLocalTransform(Matrix().Translate(Vec3f(30, -23, 30)));

  SceneNode* camera = root->GetNodeByName("camera");
  Assert(camera);
  // TODO turned off for now
//  camera->AddChild(md2node);
}

void GSPages::OnActive()
{
  // This WON'T be called if we resume from pause, there is a confirm dialog
  //  in this state.

  GSBase3d::OnActive();

  ReloadGui();
  //PrintGui(m_gui);

  // Get general user config, just a convenience, it lives in the User Profile.
  m_userConfig = TheUserProfile()->GetConfigForTopic(KEY_GENERAL);

  std::cout << "OnActive calling NextPage...\n";

  NextPage();
}

void GSPages::OnDeactive()
{
  GSBase3d::OnDeactive();
  
  SetPage(nullptr);
  
  // Save changes to user profile (questions seen, etc)
  TheUserProfile()->Save();
  // Just in case we switch users or something happens to invalidate this
  m_userConfig = nullptr;
}

void GSPages::HideTickAndCross()
{
  GuiElement* tick = GetElementByName(m_gui, "tick");
  GuiElement* cross = GetElementByName(m_gui, "cross");
  tick->ResetAnimation();
  cross->ResetAnimation();
  tick->SetVisible(false);
  cross->SetVisible(false);
}

void GSPages::NextPage()
{
  std::cout << "NextPage\n";

  // The current Topic has sequence of Pages to display.
  Course* course = GetCourse();
  Assert(course);
  // This is a bit crap - get the current Topic num from prev state, which knows it.
  Topic* topic = course->GetTopic(TheGSTopicStart::Instance()->GetTopic());
  Assert(topic);
  m_maxNumPagesThisSession = topic->GetNumPages();

  // Have we got more pages, or are we done?
  if (m_numPagesShown >= m_maxNumPagesThisSession)
  {
    // Done, go to Topic successfully completed, or unsuccessfully completed.
    // (Can use the same state?)
    GSTopicEnd* gs = TheGSTopicEnd::Instance();
    gs->SetScore(m_numCorrectThisSession, m_maxNumPagesThisSession);
    TheGame::Instance()->SetCurrentState(gs);
    return;
  }

  Page* page = topic->GetPage(m_numPagesShown);
  // Page reads/writes config file to load/save user state
  ConfigFile* cf = TheUserProfile()->GetConfigForTopic(topic->GetId());
  page->SetConfigFile(cf);
  SetPage(page);
  
  m_numPagesShown++;
  std::cout << "Increased m_numPagesShown to: " << m_numPagesShown << "\n";

  HideTickAndCross();

  // Rub out blackboard - resset anim
  GuiElement* ruboutAnim = GetElementByName(m_gui, "blackboard-erase");
  Assert(ruboutAnim);
  ruboutAnim->SetVisible(false);
  ruboutAnim->ResetAnimation();

  // Show number of pages, num correct, num incorrect, etc. These GUI elements
  //  should be in the top bar.
  IGuiText* numPagesText = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "num-pages-text"));
  Assert(numPagesText);
  // Show number of pages or number of correct answers?
  std::string s = ToString(m_numCorrectThisSession) + "/" + ToString(m_maxNumPagesThisSession);
  numPagesText->SetText(s);

  // Start pie slice colour anim
  GuiDecAnimation* sliceColourAnim = dynamic_cast<GuiDecAnimation*>(
    GetElementByName(m_gui, "anim-pie-colour-" + ToString(m_numPagesShown)));
  sliceColourAnim->SetEaseType(GuiDecAnimation::EaseType::EASE_TYPE_ONE);

  ShowHints();
}

void GSPages::SetPie(int n, const Colour& col)
{
  // Stop anim
  GuiDecAnimation* sliceColourAnim = dynamic_cast<GuiDecAnimation*>(GetElementByName(m_gui, "anim-pie-colour-" + ToString(n)));
  sliceColourAnim->SetEaseType(GuiDecAnimation::EaseType::EASE_TYPE_ZERO);

  // Set slice colour
  GuiDecColour* sliceColour = dynamic_cast<GuiDecColour*>(GetElementByName(m_gui, "colour-pie" + ToString(n)));
  Assert(sliceColour);
  sliceColour->SetColour(col, 0); // set both colours, anim value is between 0..1 
  sliceColour->SetColour(col, 1);
}

void GSPages::Draw2d()
{
  UseVertexColourShader();
  m_page->Draw();

  // Common GUI goes on top or under? We want tick/cross to go on top.
  GSBase3d::Draw2d();
}

void GSPages::Update()
{
  GSBase3d::Update();
  m_page->Update();
}

// Load list of pages from file?
bool GSPages::Load(const std::string& filename)
{
  return true;
}

bool GSPages::OnKeyEvent(const KeyEvent& ke)
{
  if (GSBase3d::OnKeyEvent(ke))
  {
    return true;
  }

#ifdef _DEBUG
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR)
  {
    switch (ke.key)
    {
      case 'p':
      {
        // Show page GUI tree
        if (m_page && m_page->GetGui())
        {
          PrintGui(m_page->GetGui());
        }
        else
        {
          std::cout << "Null page GUI!\n";
        }
        break;
      } // case
    } // switch
  } // if

#endif
  return false;
}

void GSPages::SetPage(Page* p)
{
  if (m_page)
  {
    m_page->OnDeactive();
  }

  m_page = p;

  if (m_page)
  {
    m_page->SetGameState(this);
    // Activate new page
    m_page->OnActive();
  }
}

void GSPages::OnPlayerMadeChoice()
{
  GuiElement* speechBubble = GetElementByName(m_gui, "speech-bubble");
  speechBubble->SetVisible(true);

  // Disable everything except the speech bubble OK button
  SetButtonEnabled("pause-button", false);
  SetButtonEnabled("hint-button", false);
  m_page->SetIsEnabled(false);
}

void GSPages::OnCorrect(const Vec2f& choicePos)
{
  //OnPlayerMadeChoice();
//  SetButtonEnabled("pause-button", false);
//  SetButtonEnabled("hint-button", false);
//  m_page->SetIsEnabled(false);

  GuiElement* tick = GetElementByName(m_gui, "tick");
  tick->SetVisible(true);
  tick->SetLocalPos(choicePos);

  // Rub out blackboard? - OK as answer correct?
  //GuiElement* ruboutAnim = GetElementByName(m_gui, "blackboard-erase");
  //Assert(ruboutAnim);
  //ruboutAnim->SetVisible(true);

  // TODO TEMP TEST
  // Increase hint count, this is until we have a better balanced system
  Hints::Inc();
  ShowHints();

  // Happy sound
  PlayWav(WAV_CORRECT);

  // Add to profile/score
  m_numCorrectThisSession++;
  SetPie(m_numPagesShown, Colour(0.f, 1.f, 0.f, 1.f));

  std::cout << "Correct: set pie slice " << m_numPagesShown << "\n";

  // Send message to do what we do when we click the OK button on
  //  the speech bubble, after a delay. (TODO balance delay)
  TheMessageQueue::Instance()->Add(new FuncMsg(GoToNextPage,
    SecondsFromNow(NEXT_PAGE_TIME)));

  LurkMsg lm(Lookup("Correct!"), Colour(1, 1, 1, 1), Colour(0, 1, 0.25f, 1), AMJU_TOP, PAGE_LURK_TIME);
  TheLurker::Instance()->Queue(lm);
}

void GSPages::OnIncorrect(const Vec2f& choicePos)
{
  //OnPlayerMadeChoice();

  m_page->ShowCorrectAnswer();

  GuiElement* cross = GetElementByName(m_gui, "cross");
  cross->SetVisible(true);
  cross->SetLocalPos(choicePos);

  // Unhappy sound
  PlayWav(WAV_INCORRECT);

  m_numIncorrectThisSession++;
  SetPie(m_numPagesShown, Colour(1.f, 0.f, 0.f, 1.f));

  std::cout << "Incorrect: set pie slice " << m_numPagesShown << "\n";

  LurkMsg lm(Lookup("Incorrect!"), Colour(1, 1, 1, 1), Colour(1, 0, 0, 1), AMJU_TOP, PAGE_LURK_TIME);
  TheLurker::Instance()->Queue(lm);
}

void GSPages::OnHint()
{
  // Decrement hints left
  if (Hints::Get() < 1)
  {
    // No hints sound/anim?
    // TODO

    return;
  }

  if (!m_page->CanGetHint())
  {
    return;
  }

  Hints::Dec();
  ShowHints();

  m_page->OnHint(); // show context-sensitive hint
}

void GSPages::SetButtonEnabled(const std::string& buttonName, bool enabled)
{
  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, buttonName));
  Assert(button);
  button->SetIsEnabled(enabled);

  // Set colour (assumes immediate parent is colour decorator)
  GuiElement* col = button->GetParent();
  Assert(col);
  col->Animate(enabled ? 0.f : 1.f); // i.e. enabled = first colour, disabled = 2nd colour
}

void GSPages::OnPause()
{
  // Bring on pause dialog
  GuiDecAnimation* anim = dynamic_cast<GuiDecAnimation*>(
    GetElementByName(m_gui, "quit-confirm-swipe"));
  Assert(anim);
  anim->SetEaseType(GuiDecAnimation::EaseType::EASE_TYPE_ONE);

  anim = dynamic_cast<GuiDecAnimation*>(
    GetElementByName(m_gui, "swipe-anim"));
  Assert(anim);
  anim->ResetAnimation();
  anim->SetIsReversed(false);
  
  // Disable pause and hint buttons
  SetButtonEnabled("pause-button", false);
  SetButtonEnabled("hint-button", false);

  // Disable page contents
  m_page->SetIsEnabled(false); 
}
 
void GSPages::OnSpeechBubbleOK()
{
  std::cout << "OnSpeechBubbleOK\n";

  // Hide speech bubble
  GuiElement* speechBubble = GetElementByName(m_gui, "speech-bubble");
  speechBubble->SetVisible(false);

  HideTickAndCross();

  SetButtonEnabled("pause-button", true);
  SetButtonEnabled("hint-button", true);

  m_page->SetIsEnabled(true);

  // Go to next page (or end of state), after a delay to show the board
  //  getting rubbed out.
  GoToNextPage();
  //TheMessageQueue::Instance()->Add(new FuncMsg(GoToNextPage, SecondsFromNow(NEXT_PAGE_TIME)));
}

void GSPages::OnQuitConfirmCancel()
{
  // Swipe off confirm dialog
  GuiDecAnimation* anim = dynamic_cast<GuiDecAnimation*>(
    GetElementByName(m_gui, "swipe-anim"));
  Assert(anim);
  anim->ResetAnimation();
  anim->SetIsReversed(true);

  SetButtonEnabled("pause-button", true);
  SetButtonEnabled("hint-button", true);

  // Re-enable page contents
  m_page->SetIsEnabled(true); 
}

void GSPages::OnMusicKbEvent(const MusicKbEvent& e)
{
  if (m_page)
  {
    m_page->OnMusicKbEvent(e);
  }
}

}

