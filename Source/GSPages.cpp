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

#include "CentreMsg.h"
#include "Consts.h"
#include "Course.h"
#include "GSMainCorridor.h"
#include "GSPages.h"
#include "GSPause.h"
#include "GSTopicStart.h"
#include "GSTopicEnd.h"
#include "GuiLineDrawing.h"
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

GSPages::GSPages()
{
  m_guiFilename = "Gui/gs_pages_landscape.txt";
  m_sceneFilename = "Scene/room1-scene.txt";
}

void GSPages::ShowGuiElement(const std::string& elemName, bool showNotHide)
{
  GuiElement* elem = GetElementByName(m_gui, elemName);
  Assert(elem);
  elem->SetVisible(showNotHide);
}

void GSPages::StartTopic(int topicNum)
{
  m_numPagesShown = 0;
  m_numCorrectThisSession = 0;
  m_numIncorrectThisSession = 0;
  m_scoreThisSession = 0;
  m_scoreMultiplier = 1;
  m_livesThisSession = 3;

  // TODO Get this from Topic. 
  m_maxNumPagesThisSession = 10;

  TheUserProfile()->SetTopicScore(0);
}

void GSPages::ShowHints()
{
  IGuiText* hintCounter = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "hint-counter"));
  Assert(hintCounter);
  int hints = TheUserProfile()->GetHints();
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

void GSPages::UpdateHud()
{
  // Score
  IGuiText* scoreText = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "score-text"));
  Assert(scoreText);
  // Show number of pages or number of correct answers?
  std::string s = ToString(m_scoreThisSession);
  scoreText->SetText(s);

  // Lives
  IGuiText* livesText = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "num-lives-text"));
  Assert(livesText);
  // Show number of pages or number of correct answers?
  s = ToString(m_livesThisSession);
  livesText->SetText(s);

  // TODO Combo
}

void GSPages::NextPage()
{
  // Have we got more pages, or are we done?
  if (m_livesThisSession < 1) ////m_numPagesShown >= m_maxNumPagesThisSession)
  {
    // Done, go to Topic successfully completed, or unsuccessfully completed.
    // (Can use the same state?)
    GSTopicEnd* gs = TheGSTopicEnd::Instance();
    TheUserProfile()->SetTopicScore(m_scoreThisSession);

    TheGame::Instance()->SetCurrentState(gs);
    return;
  }

  // The current Topic has sequence of Pages to display.
  Course* course = GetCourse();
  Assert(course);
  Topic* topic = course->GetTopic(TheUserProfile()->GetCurrentTopic());
  Assert(topic);
  m_maxNumPagesThisSession = topic->GetNumPages();

  // Rotate pages in topic, until we run out of lives.
  int pageNum = m_numPagesShown % m_maxNumPagesThisSession;

  Page* page = topic->GetPage(pageNum);
  // Page reads/writes config file to load/save user state
  ConfigFile* cf = TheUserProfile()->GetConfigForTopic(topic->GetId());
  page->SetConfigFile(cf);
  SetPage(page);
  
  m_numPagesShown++;

  // Rub out blackboard - reset anim
  GuiElement* ruboutAnim = GetElementByName(m_gui, "blackboard-erase");
  Assert(ruboutAnim);
  ruboutAnim->SetVisible(false);
  ruboutAnim->ResetAnimation();

  UpdateHud();

  ShowHints();
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

void GSPages::OnCorrect()
{
  // Happy sound
  PlayWav(WAV_CORRECT);

  // Add to profile/score
  m_numCorrectThisSession++;

  m_scoreThisSession += m_scoreMultiplier;
  // TODO score anim
  UpdateHud();

  Page::SendNextPageMessage();

  PLurkMsg lm = new LurkMsg(Lookup("$$$121" /* Correct! */), 
    GetColour(COLOUR_TEXT),
    GetColour(COLOUR_CORRECT),
    AMJU_TOP, PAGE_LURK_TIME);

  TheLurker::Instance()->Queue(lm);
}

void GSPages::OnIncorrect()
{
  m_page->ShowCorrectAnswer();

  // Unhappy sound
  PlayWav(WAV_INCORRECT);

  m_numIncorrectThisSession++;

  m_livesThisSession--;
  UpdateHud();


  // Do this in m_page->ShowCorrectAnswer() if required
  //LurkMsg lm(Lookup("Incorrect!"), 
  //  GetColour(COLOUR_TEXT),
  //  GetColour(COLOUR_INCORRECT),
  //  AMJU_TOP, PAGE_LURK_TIME);
  //TheLurker::Instance()->Queue(lm);
}

void GSPages::OnHint()
{
  // Decrement hints left
  auto profile = TheUserProfile();
  if (profile->GetHints() < 1)
  {
    // No hints sound/anim?
    // TODO

    return;
  }

  if (!m_page->CanGetHint())
  {
    return;
  }

  profile->DecHints();
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
  // Lurk messages are modal, so no need to diable page
  ShowYesNo(
    Lookup("$$$90") /* Sure you want to quit? */,
    GetColour(COLOUR_TEXT),
    GetColour(COLOUR_CONFIRM_QUIT),
    Amju::OnQuitConfirmCancel,
    Amju::OnQuitConfirmOK);
}

void GSPages::OnQuitConfirmCancel()
{
  // Nothing to do, modal Lurk dialog returns control back to us.
}

void GSPages::OnMusicKbEvent(const MusicKbEvent& e)
{
  if (m_page)
  {
    m_page->OnMusicKbEvent(e);
  }
}

}

