// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <AmjuGL.h>
#include <CursorManager.h>
#include <GuiButton.h>
#include <GuiComposite.h>
#include <GuiDecAnimation.h>
#include <ROConfig.h>
#include "GSBase.h"
#include "LurkMsg.h"
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

void GSBase::Draw2d() 
{
  AmjuGL::SetClearColour(Colour(.95f, .95f, .95f, 1.f));

  if (m_gui)
  { 
    UseVertexColourShader();
    m_gui->Draw();
  }

  TheLurker::Instance()->Draw();

#ifdef GEKKO
  TheCursorManager::Instance()->Draw();
#endif
}

void GSBase::OnActive() 
{
  GameState::OnActive();

  m_gui = LoadGui(m_guiFilename);
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
  OnActive();
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

#endif
  return false;
}

void GSBase::ScrollRight()
{
  GuiDecAnimation* anim = dynamic_cast<GuiDecAnimation*>(GetElementByName(m_gui, "scroll-right-trigger"));
  anim->SetEaseType(GuiDecAnimation::EaseType::EASE_TYPE_ONE);
}

void GSBase::ScrollLeft()
{
  GuiDecAnimation* anim = dynamic_cast<GuiDecAnimation*>(GetElementByName(m_gui, "scroll-left-trigger"));
  anim->SetEaseType(GuiDecAnimation::EaseType::EASE_TYPE_ONE);
}

void GSBase::ScrollUp()
{
  GuiDecAnimation* anim = dynamic_cast<GuiDecAnimation*>(GetElementByName(m_gui, "scroll-up-trigger"));
  anim->SetEaseType(GuiDecAnimation::EaseType::EASE_TYPE_ONE);
}

void GSBase::ScrollDown()
{
  GuiDecAnimation* anim = dynamic_cast<GuiDecAnimation*>(GetElementByName(m_gui, "scroll-down-trigger"));
  anim->SetEaseType(GuiDecAnimation::EaseType::EASE_TYPE_ONE);
}
}

