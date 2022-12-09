// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <AmjuGL.h>
#include <ConfigFile.h>
#include <Game.h>
#include <GuiText.h>
#include <MessageQueue.h>
#include "GSMainMenu.h"
#include "GSPages.h"
#include "GSPause.h"
#include "GuiLineDrawing.h"
#include "PageMusicalTerm.h"
#include "UserProfile.h"

namespace Amju
{
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

void GSPages::OnActive()
{
  // This could be a resume from pause, so we don't reset the topic progress here.

  GSBase::OnActive();

  GuiElement* elem = GetElementByName(m_gui, "pause-button");
  Assert(elem);
  elem->SetCommand(OnPause);

  NextPage();
}

void GSPages::OnDeactive()
{
  GSBase::OnDeactive();
  m_pages.clear();
}

void GSPages::NextPage()
{
  m_numPagesShown++;
  // Have we got more pages, or are we done?
  if (m_numPagesShown > m_maxNumPagesThisSession)
  {
    // Done, go to Topic successfully completed, or unsuccessfully completed.
    // (Can use the same state?)

    // TODO TEMP TEST
    TheGame::Instance()->SetCurrentState(TheGSMainMenu::Instance());
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

  TheMessageQueue::Instance()->Add(new FuncMsg(GoToNextPage, SecondsFromNow(1.0f)));
}

void GSPages::OnIncorrect()
{
  GuiElement* cross = GetElementByName(m_gui, "cross");
  cross->SetVisible(true);

  // Unhappy sound
  // TODO

  m_numIncorrectThisSession++;

  TheMessageQueue::Instance()->Add(new FuncMsg(GoToNextPage, SecondsFromNow(1.0f)));
}

void GSPages::OnHint()
{
  // Decrement hints left

  m_pages[m_currentPage]->OnHint(); // show context-sensitive hint
}

}

