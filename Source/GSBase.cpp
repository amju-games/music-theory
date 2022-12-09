// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <AmjuGL.h>
#include <CursorManager.h>
#include <GuiButton.h>
#include <GuiComposite.h>
#include <GuiDecAnimation.h>
#include "GSBase.h"
#include "PrintGui.h"
#include "ShareManager.h"

namespace Amju
{
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
  if (m_gui)
  {
    m_gui->Update();
  }
}

void GSBase::UseVertexColourShader()
{
#if defined(WIN32) || defined(MACOSX)
  // Set shader for desktop GL - fixed function doesn't seem to treat vertex colours the
  //  way we want (i.e. multiply by currently active colour)
  static Shader* sh = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/gui");
  Assert(sh);
  AmjuGL::UseShader(sh);
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
  // Reload score
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR &&
    (ke.key == 'r' || ke.key == 'R'))
  {
    std::cout << "Reloading\n";
    ReloadGui();
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

