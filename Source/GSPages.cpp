// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <AmjuGL.h>
#include <ConfigFile.h>
#include <Game.h>
#include <GuiButton.h>
#include <GuiDecAnimation.h>
#include <GuiDecColour.h>
#include <GuiText.h>
#include <MessageQueue.h>
#include "Course.h"
#include "GSMainMenu.h"
#include "GSPages.h"
#include "GSPause.h"
#include "GSTopicStart.h"
#include "GSTopicEnd.h"
#include "GuiLineDrawing.h"
#include "Keys.h"
#include "PrintGui.h"
#include "Topic.h"
#include "UserProfile.h"

namespace Amju
{
static const char* HINTS_AVAILABLE_KEY = "hints_avail";
// Start life with 3 hints
const int DEFAULT_HINTS_AVAIL = 3;

static void OnQuitConfirmOK(GuiElement*)
{
  TheGame::Instance()->SetCurrentState(TheGSMainMenu::Instance());
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
static void GoToNextPage()
{
  GSPages* pages = TheGSPages::Instance();
  // Sanity check: this only makes sense if GSPages is active
  if (TheGame::Instance()->GetState() == pages)
  {
    pages->NextPage();
  }
}

GSPages::GSPages()
{
  m_guiFilename = "Gui/gs_pages_landscape.txt";
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
  hintCounter->SetText(ToString(m_numHintsAvailable));
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

  ShowHints();
}

void GSPages::OnActive()
{
  // This WON'T be called if we resume from pause, there is a confirm dialog
  //  in this state.

  GameState::OnActive();
  ReloadGui();
  //PrintGui(m_gui);

  // Get general user config, just a convenience, it lives in the User Profile.
  m_userConfig = TheUserProfile()->GetConfigForTopic(KEY_GENERAL);
  // How many Hints are available?
  m_numHintsAvailable = m_userConfig->GetInt(HINTS_AVAILABLE_KEY, DEFAULT_HINTS_AVAIL);

  ShowHints();

  NextPage();
}

void GSPages::OnDeactive()
{
  GSBase::OnDeactive();
  
  //m_pages.clear();
  m_page = nullptr;
  
  // Save changes to user profile (questions seen, etc)
  TheUserProfile()->Save();
  // Just in case we switch users or something happens to invalidate this
  m_userConfig = nullptr;
}

void GSPages::NextPage()
{
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
  AddPage(page);
  
  // Activate new page
  page->OnActive();

  m_numPagesShown++;

  // Tick/cross 
  GuiElement* tick = GetElementByName(m_gui, "tick");
  GuiElement* cross = GetElementByName(m_gui, "cross");
  tick->ResetAnimation();
  cross->ResetAnimation();
  tick->SetVisible(false);
  cross->SetVisible(false);

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
  AmjuGL::PushMatrix();
  // Bad idea douchebag, fucks up coords for clicking
  //AmjuGL::Translate(0, -0.2f, 0);
  m_page->Draw();
  AmjuGL::PopMatrix();

  // Common GUI goes on top or under? We want tick/cross to go on top.
  GSBase::Draw2d();
}

void GSPages::Update()
{
  GSBase::Update();
  m_page->Update();
}

// Load list of pages from file?
bool GSPages::Load(const std::string& filename)
{
  return true;
}

bool GSPages::OnKeyEvent(const KeyEvent& ke)
{
  if (GSBase::OnKeyEvent(ke))
  {
    return true;
  }

#ifdef _DEBUG
  // Show page GUI tree
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR &&
    (ke.key == 'p' || ke.key == 'P'))
  {
    if (m_page && m_page->GetGui())
    {
      PrintGui(m_page->GetGui());
    }
    else
    {
      std::cout << "Null page GUI!\n";
    }
  }

#endif
  return false;
}

void GSPages::AddPage(Page* p)
{
  p->SetGameState(this);
  m_page = p;
}

void GSPages::OnCorrect()
{
  GuiElement* tick = GetElementByName(m_gui, "tick");
  tick->SetVisible(true);

  // Happy sound
  // TODO

  // Add to profile/score
  m_numCorrectThisSession++;
  SetPie(m_numPagesShown, Colour(0.f, 1.f, 0.f, 1.f));
  TheMessageQueue::Instance()->Add(new FuncMsg(GoToNextPage, SecondsFromNow(1.0f)));
}

void GSPages::OnIncorrect()
{
  GuiElement* cross = GetElementByName(m_gui, "cross");
  cross->SetVisible(true);

  // Unhappy sound
  // TODO

  m_numIncorrectThisSession++;
  SetPie(m_numPagesShown, Colour(1.f, 0.f, 0.f, 1.f));
  TheMessageQueue::Instance()->Add(new FuncMsg(GoToNextPage, SecondsFromNow(1.0f)));
}

void GSPages::OnHint()
{
  // Decrement hints left
  if (m_numHintsAvailable < 1)
  {
    // No hints sound/anim?
    // TODO

    return;
  }
  m_numHintsAvailable--;
  m_userConfig->SetInt(HINTS_AVAILABLE_KEY, m_numHintsAvailable);
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

