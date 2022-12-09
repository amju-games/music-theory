// * Amjula music theory *
// (c) Copyright 2017-2018 Jason Colman

#include <GuiButton.h>
#include "ColourPicker.h"

namespace Amju
{
const char* ColourPicker::NAME = "colour-picker";

bool ColourPicker::Load(File* f)
{
  if (!GuiDecInclude::Load(f))
  {
    return false;
  }

  SetName("colour-picker");

  auto onButton = [this](GuiElement* elem) 
  {
    GuiButton* button = dynamic_cast<GuiButton*>(elem);
    Assert(button);
    m_onColourPickFunc(button->GetButtonColour());
  };

  // Set command for all buttons
  // TODO This is just a basic POC
  for (int i = 0; i < 16; i++)
  {
    GuiButton* b = dynamic_cast<GuiButton*>(GetElementByName("button-colour-" + ToString(i)));
    Assert(b);
    b->SetOnPressedDownFunc(onButton);
  }

  return true;
}
}
