// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <AmjuGL.h>
#include <ConfigFile.h>
#include <Game.h>
#include <GuiDecAnimation.h>
#include <GuiDecColour.h>
#include <GuiText.h>
#include <MessageQueue.h>
#include "GSPages.h"
#include "GSPause.h"
#include "GSTopicEnd.h"
#include "GuiLineDrawing.h"
#include "PageMusicalTerm.h"
#include "UserProfile.h"

namespace Amju
{
static const char* HINTS_AVAILABLE_KEY = "hints_avail";
// Start life with 3 hints
const int DEFAULT_HINTS_AVAIL = 3;

static void OnPause(GuiElement*)
{
  GSPause* gs = TheGSPause::Instance();
  gs->SetPrevState(TheGSPages::Instance());
  TheGame::Instance()->SetCurrentState(gs);
}

// Called from timed FuncMessage
static void GoToNextPage()
{
  TheGSPages::Instance()->NextPage();
}

GSPages::GSPages()
{
  m_guiFilename = "Gui/gs_pages.txt";
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
  GuiText* hintCounter = dynamic_cast<GuiText*>(GetElementByName(m_gui, "hint-counter"));
  Assert(hintCounter);
  hintCounter->SetText(ToString(m_numHintsAvailable));
}

void GSPages::OnActive()
{
  // This could be a resume from pause, so we don't reset the topic progress here.

  GSBase::OnActive();

  GuiElement* elem = GetElementByName(m_gui, "pause-button");
  Assert(elem);
  elem->SetCommand(OnPause);

  // Get general user config, just a convenience, it lives in the User Profile.
  m_userConfig = TheUserProfile()->GetConfigForTopic("general");
  // How many Hints are available?
  m_numHintsAvailable = m_userConfig->GetInt(HINTS_AVAILABLE_KEY, DEFAULT_HINTS_AVAIL);

  ShowHints();

  NextPage();
}

void GSPages::OnDeactive()
{
  GSBase::OnDeactive();
  m_pages.clear();
  // Save changes to user profile (questions seen, etc)
  TheUserProfile()->Save();
  // Just in case we switch users or something happens to invalidate this
  m_userConfig = nullptr;
}

void GSPages::NextPage()
{
  m_numPagesShown++;
  // Have we got more pages, or are we done?
  if (m_numPagesShown > m_maxNumPagesThisSession)
  {
    // Done, go to Topic successfully completed, or unsuccessfully completed.
    // (Can use the same state?)
    GSTopicEnd* gs = TheGSTopicEnd::Instance();
    gs->SetScore(m_numCorrectThisSession, m_maxNumPagesThisSession);
    TheGame::Instance()->SetCurrentState(gs);
    return;
  }

  // TODO The current Topic should know what kind of page to create
  //  (we might use separate factory?)
  Page* page = new PageMusicalTerm;
  ConfigFile* cf = TheUserProfile()->GetConfigForTopic("todo");
  page->SetConfigFile(cf);
  AddPage(page);
  m_currentPage = 0;

  // Activate new page
  m_pages[m_currentPage]->OnActive();

  // Tick/cross 
  GuiElement* tick = GetElementByName(m_gui, "tick");
  GuiElement* cross = GetElementByName(m_gui, "cross");
  tick->ResetAnimation();
  cross->ResetAnimation();
  tick->SetVisible(false);
  cross->SetVisible(false);

  // Show number of pages, num correct, num incorrect, etc. These GUI elements
  //  should be in the top bar.
  GuiText* numPagesText = (GuiText*)GetElementByName(m_gui, "num-pages-text");
  Assert(numPagesText);
  // Show number of pages or number of correct answers?
  std::string s = ToString(m_numCorrectThisSession) + "/" + ToString(m_maxNumPagesThisSession);
  numPagesText->SetText(s);
}

void GSPages::SetPie(int n, const Colour& col)
{
  GuiDecColour* sliceColour = dynamic_cast<GuiDecColour*>(GetElementByName(m_gui, "colour-pie" + ToString(n)));
  Assert(sliceColour);
  sliceColour->SetColour(col);
}

void GSPages::Draw2d()
{
  AmjuGL::PushMatrix();
  // Bad idea douchebag, fucks up coords for clicking
  //AmjuGL::Translate(0, -0.2f, 0);
  m_pages[m_currentPage]->Draw(); 
  AmjuGL::PopMatrix();

  // Common GUI goes on top or under? We want tick/cross to go on top.
  GSBase::Draw2d();
}

void GSPages::Update()
{
  GSBase::Update();
  m_pages[m_currentPage]->Update();
}

// Load list of pages from file?
bool GSPages::Load(const std::string& filename)
{
  return true;
}

void GSPages::AddPage(Page* p)
{
  p->SetGameState(this);
  m_pages.push_back(p);
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

  m_pages[m_currentPage]->OnHint(); // show context-sensitive hint
}

}

