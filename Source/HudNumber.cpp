// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <GuiDecAnimation.h>
#include <GuiText.h>
#include <Sign.h>
#include <StringUtils.h>
#include "HudNumber.h"

namespace Amju
{
void HudNumber::SetGuiElement(
  PGuiElement guiRoot, 
  const std::string& guiTextElementName, 
  const std::string& guiAnimElementName)
{
  m_guiTextElement = guiRoot->GetElementByName(guiTextElementName);
  Assert(m_guiTextElement);
  
  if (!guiAnimElementName.empty())
  {
    m_guiAnimElement = guiRoot->GetElementByName(guiAnimElementName);
    Assert(m_guiAnimElement);
  }
}

void HudNumber::ResumeAfterPause()
{
  Reset(m_internalNumber);
}

void HudNumber::Reset(int initialValue)
{
  m_displayedNumber = initialValue;
  m_internalNumber = initialValue;
  m_increment = 0;

  IGuiText* text = dynamic_cast<IGuiText*>(m_guiTextElement.GetPtr());
  Assert(text); 
  text->SetText(m_prefix + std::to_string(initialValue) + m_suffix);
}

void HudNumber::Update()
{
  if (m_increment != 0)
  {
    int diffBefore = m_internalNumber - m_displayedNumber;
    m_displayedNumber += m_increment;
    int diffAfter = m_internalNumber - m_displayedNumber; 
    if (Sign(diffBefore) != Sign(diffAfter))
    {
      m_displayedNumber = m_internalNumber;
      m_increment = 0;
    }

    IGuiText* text = dynamic_cast<IGuiText*>(m_guiTextElement.GetPtr());
    Assert(text); 
    text->SetText(m_prefix + std::to_string(m_displayedNumber) + m_suffix);
  }
}

void HudNumber::Add(int amountToAdd, int numFrames)
{
  if (amountToAdd == 0)
  {
    return;
  }

  m_internalNumber += amountToAdd;
  m_increment = (m_internalNumber - m_displayedNumber) / numFrames;
  if (m_increment == 0)
  {
    m_increment = Sign(amountToAdd);
  }

  ResetAnimation();
}

void HudNumber::ResetAnimation()
{
  if (m_guiAnimElement)
  {
    auto anim = dynamic_cast<GuiDecAnimation*>(m_guiAnimElement.GetPtr());
    Assert(anim);
    anim->ResetAnimation();
  }
}

/*
void NumUpdate(PGuiElement gui, const std::string& elemName, int value)
{
  NumUpdate(gui, elemName, ToString(value));
}

void NumUpdate(PGuiElement gui, const std::string& elemName, const std::string& value)
{
  // Find element
  IGuiText* text = dynamic_cast<IGuiText*>(gui->GetElementByName(elemName));
  Assert(text);

  if (text->GetText() != value)
  {
    text->SetText(value);

    GuiDecAnimation* anim = dynamic_cast<GuiDecAnimation*>(gui->GetElementByName(elemName + "-anim-trigger"));
    if (anim)
    {
      anim->ResetAnimation();
    }
  }
}
*/
}
