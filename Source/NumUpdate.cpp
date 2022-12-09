// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GuiDecAnimation.h>
#include <GuiText.h>

namespace Amju
{
void NumUpdate(PGuiElement gui, const std::string& elemName, int value)
{
  // Find element
  IGuiText* text = dynamic_cast<IGuiText*>(gui->GetElementByName(elemName));
  Assert(text);
  text->SetText(ToString(value));

  text = dynamic_cast<IGuiText*>(gui->GetElementByName(elemName + "-shadow"));
  if (text)
  {
    text->SetText(ToString(value));
  }

  GuiDecAnimation* anim = dynamic_cast<GuiDecAnimation*>(gui->GetElementByName(elemName + "-anim-trigger"));
  if (anim)
  {
    anim->ResetAnimation();
  }
}
}
