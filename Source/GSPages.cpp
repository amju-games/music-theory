// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <AmjuGL.h>
#include <Game.h>
#include <MessageQueue.h>
#include "GuiLineDrawing.h"
#include "GSPages.h"
#include "GSPause.h"
#include "PageMusicalTerm.h"

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

void GSPages::OnActive()
{
  // This could be a resume from pause, so we don't reset the topic progress here.

  GSBase::OnActive();

  GuiElement* elem = GetElementByName(m_gui, "pause-button");
  Assert(elem);
  elem->SetCommand(OnPause);

  NextPage();
}

void GSPages::NextPage()
{
  AddPage(new PageMusicalTerm);
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

  TheMessageQueue::Instance()->Add(new FuncMsg(GoToNextPage, SecondsFromNow(1.0f)));
}

void GSPages::OnIncorrect()
{
  GuiElement* cross = GetElementByName(m_gui, "cross");
  cross->SetVisible(true);

  // Unhappy sound
  // TODO

  TheMessageQueue::Instance()->Add(new FuncMsg(GoToNextPage, SecondsFromNow(1.0f)));
}

void GSPages::OnHint()
{
  // Decrement hints left

  m_pages[m_currentPage]->OnHint(); // show context-sensitive hint
}

}

