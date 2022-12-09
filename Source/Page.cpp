// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <ConfigFile.h>
#include <GameState.h>
#include <GuiDecAnimation.h>
#include "GSPages.h"
#include "Page.h"

namespace Amju
{
static void OnHint(GuiElement* hintButton)
{
  TheGSPages::Instance()->OnHint();
}

void Page::SetConfigFile(ConfigFile* cf)
{
  m_config = cf;
}

void Page::SetDictionaryName(const std::string& dictionaryName)
{
  m_dictionaryFilename = dictionaryName;
}

GuiButton* Page::GetHintButton() const
{
  GuiElement* hint = GetElementByName(TheGSPages::Instance()->GetGui(), "hint-button");
  return dynamic_cast<GuiButton*>(hint);
}

void Page::SetIsEnabled(bool enabled)
{
  if (enabled)
  {
    // Re-enable buttons
    GuiDecAnimation* anim = dynamic_cast<GuiDecAnimation*>(
      GetElementByName(m_gui, "anim-colour-fade-whole-page"));
    Assert(anim);
    anim->ResetAnimation();
    anim->SetIsReversed(true);
  }
  else
  {
    // Fade page contents down, which disables the buttons
    GuiDecAnimation* anim = dynamic_cast<GuiDecAnimation*>(
      GetElementByName(m_gui, "fade-out-whole-page"));
    Assert(anim);
    anim->ResetAnimation();
    anim->SetIsReversed(false);
    anim->SetEaseType(GuiDecAnimation::EaseType::EASE_TYPE_ONE);

    anim = dynamic_cast<GuiDecAnimation*>(
      GetElementByName(m_gui, "anim-colour-fade-whole-page"));
    Assert(anim);
    anim->SetIsReversed(false);
  }
}

void Page::OnActive()
{
  // TODO append orientation to gui name
  m_gui = LoadGui("Gui/page_" + m_guiName + ".txt");

  // Adjust position so it sits under the top bar
  m_gui->SetLocalPos(Vec2f(0, -0.2f));

  // Set command for common buttons
  GuiElement* hint = GetHintButton();
  if (hint)
  {
    hint->SetCommand(Amju::OnHint);
  }
}

GuiElement* Page::GetGui()
{
  return m_gui;
}
  
void Page::Draw()
{
  m_gui->Draw();
}

void Page::Update()
{
  m_gui->Update();
}

void Page::SetGameState(GameState* gs)
{
  m_gs = gs;
}

void Page::SetIsLearnMode(bool learn)
{
  m_isLearnMode = learn;
}

bool Page::IsLearnMode() const
{
  return m_isLearnMode;
}
}

