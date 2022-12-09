// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <AmjuGL.h>
#include <CursorManager.h>
#include <GuiDecAnimation.h>
#include "GSBase.h"

namespace Amju
{
void GSBase::Update()
{
  if (m_gui)
  {
    m_gui->Update();
  }
}

void GSBase::Draw2d() 
{
  AmjuGL::SetClearColour(Colour(.95f, .95f, .95f, 1.f));
//  AmjuGL::SetClearColour(Colour(.1f, .1f, .1f, 1.f));

  if (m_gui)
  { 
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

bool GSBase::OnKeyEvent(const KeyEvent& ke)
{
#ifdef _DEBUG
  // Reload score
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR &&
    (ke.key == 'r' || ke.key == 'R'))
  {
    std::cout << "Reloading\n";
    OnActive(); // or factor out score loading
    return true;
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

