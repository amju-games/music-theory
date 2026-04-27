// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuGL.h>
#include <CursorManager.h>
#include <GuiButton.h>
#include <GuiComposite.h>
#include <GuiDecAnimation.h>
#include <GuiMenu.h>
#include "GSBase.h"
#include "MyROConfig.h"
#include "PrintGui.h"
#include "ShareManager.h"
#include "UseVertexColourShader.h"

namespace Amju
{
static bool reload = false;

void OnShare(GuiElement*)
{
  TheShareManager::Instance()->ShareTextAndScreenshot();
}

void GSBase::HideButtons(GuiElement* elem)
{
  if (dynamic_cast<GuiButton*>(elem))
  {
    elem->SetVisible(false);
  }
  else if (GuiComposite* comp = dynamic_cast<GuiComposite*>(elem))
  {
    int n = comp->GetNumChildren();
    for (int i = 0; i < n; i++)
    {
      HideButtons(comp->GetChild(i));
    }
  }
}

GSBase* GSBase::HideButtons()
{
  HideButtons(m_gui);
  return this;
}

void GSBase::Update()
{
#ifdef _DEBUG
  if (reload)
  {
    reload = false;
    TheMessageQueue::Instance()->Clear();
    ReloadMyROConfig();
    ReloadGui();
  }
#endif

  if (m_gui)
  {
    m_gui->Update();
  }

// Not on device
#ifndef AMJU_IOS
  auto frameStatsText = 
    dynamic_cast<IGuiText*>(GetElementByName(m_gui, "frame-stats"));
  if (frameStatsText)
  {
    frameStatsText->SetText(TheGame::Instance()->GetFrameStats());
  }
#endif
}

void GSBase::Draw2d() 
{
  AmjuGL::SetClearColour(Colour(.95f, .95f, .95f, 1.f));

  if (m_gui)
  { 
    UseVertexColourShader();
    m_gui->Draw();
  }

#ifdef GEKKO
  TheCursorManager::Instance()->Draw();
#endif
}

void GSBase::OnActive() 
{
  GameState::OnActive();

  m_gui = LoadGui(m_guiFilename);
  if (!m_gui)
  {
    std::cout << "Failed to load: " << m_guiFilename << "\n";
    Assert(false);
  }
 
// Not on device
#ifndef AMJU_IOS 
  // Extra GUI, to display frame stats, etc
  auto extraGui = LoadGui("Gui/extra-gui.txt", false);
  if (extraGui)
  {
    auto newRoot = new GuiComposite;
    newRoot->AddChild(m_gui);
    newRoot->AddChild(extraGui);
    m_gui = newRoot;
  }
  else
  {
    std::cout << "Failed to load extra GUI.\n";
  }
#endif
}

GuiElement* GSBase::GetGui()
{
  return m_gui;
}

void GSBase::OnDeactive()
{
  // Anim messages in the queue need to be cleared!
  TheMessageQueue::Instance()->Clear();

  GameState::OnDeactive();
  m_gui = nullptr;
}

void GSBase::ReloadGui()
{
  OnDeactive();

  // Reload Composer list
  //GetComposerList().Load("Gui/composers.txt");

  OnActive();
}

bool GSBase::CheckForKey_B_BackToPrevState(const KeyEvent& ke)
{
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR &&
    (ke.key == 'b' || ke.key == 'B'))
  {
    auto* state = TheGame::Instance()->GetState();
    if (state->GetPrevState())
    {
      state->GoBack();
    }
    return true;
  }
  return false;
}

bool GSBase::OnKeyEvent(const KeyEvent& ke)
{
#ifdef _DEBUG
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR &&
    (ke.key == 'p' || ke.key == 'P'))
  {
    TheGame::Instance()->PauseGame();
    return true;
  }

  // Reload all resources: slow
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR &&
    (ke.key == 't' || ke.key == 'T'))
  {
    TheResourceManager::Instance()->Reload();
    return true;
  }

  // Report state of resources and AmjuGL
  // TODO Split this across different keys?
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR &&
    (ke.key == 'y' || ke.key == 'Y'))
  {
    TheResourceManager::Instance()->DebugPrint();
    AmjuGL::ReportState(std::cout);

    return true;
  }

  // Reload GUI
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR &&
    (ke.key == 'r' || ke.key == 'R'))
  {
    std::cout << "Reloading\n";
    reload = true;
    return true;
  }

  // Show GUI tree
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR &&
    (ke.key == 'g' || ke.key == 'G'))
  {
    if (m_gui)
    {
      PrintGui(m_gui);
    }
    else
    {
      std::cout << "Null GUI!\n";
    }
  }

  if (CheckForKey_B_BackToPrevState(ke))
  {
    return true;
  }

#endif
  return false;
}

const std::string& GSBase::GetGuiFilename()
{
  return m_guiFilename;
}
}

