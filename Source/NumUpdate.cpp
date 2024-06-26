// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <GuiDecAnimation.h>
#include <GuiText.h>
#include <StringUtils.h>
#include "NumUpdate.h"

namespace Amju
{
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
}
