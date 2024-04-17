// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuGL.h>
#include <CursorManager.h>
#include <GuiButton.h>
#include <GuiComposite.h>
#include <GuiDecAnimation.h>
#include <GuiMenu.h>
#include <ROConfig.h>
#include "ComposerList.h"
#include "DevMenu.h"
#include "GSAvatarMod.h"
#include "GSBase.h"
#include "GSGuiEdit.h"
#include "LurkMsg.h"
#include "Notebook.h" // TODO TEMP TEST for debugging
#include "PrintGui.h"
#include "SerialReqManager.h"
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
    ReloadROConfig();
    ReloadGui();
  }
#endif

  if (m_gui)
  {
    m_gui->Update();
  }

  TheLurker::Instance()->Update();

  TheSerialReqManager::Instance()->Update();
}

void GSBase::DrawDevMenu()
{
  auto* devMenu = GetDevMenu();
  devMenu->SetVisible(true);
  devMenu->Draw();
}

void GSBase::Draw2d() 
{
  AmjuGL::SetClearColour(Colour(.95f, .95f, .95f, 1.f));

  if (m_gui)
  { 
    UseVertexColourShader();
    m_gui->Draw();
  }

  TheLurker::Instance()->Draw();

  DrawDevMenu();

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
}

GuiElement* GSBase::GetGui()
{
  return m_gui;
}

void GSBase::OnDeactive()
{
  GameState::OnDeactive();
  m_gui = nullptr;
}

void GSBase::ReloadGui()
{
  OnDeactive();
  // Reload Composer list
  GetComposerList().Load("Gui/composers.txt");
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

  // Show notebook
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR &&
    (ke.key == 'n' || ke.key == 'N'))
  {
    TheLurker::Instance()->Queue(new Notebook);
  }

  // Avatar mod
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR &&
     (ke.key == 'a' || ke.key == 'A'))
  {
    GSAvatarMod* a = TheGSAvatarMod::Instance();
    if (a->GetPrevState() != this)
    {
      a->SetPrevState(this);
    }
    TheGame::Instance()->SetCurrentState(a);
  }

  // Gui Edit mode
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR &&
     (ke.key == 'e' || ke.key == 'E'))
  {
    GSGuiEdit* a = TheGSGuiEdit::Instance();
    if (a->GetPrevState() != this)
    {
      a->SetPrevState(this);
    }
    a->SetEditGuiFilename(m_guiFilename);
    TheGame::Instance()->SetCurrentState(a);
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

