// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <iostream>
#include <GuiComposite.h>
#include "PrintGui.h"

namespace Amju
{
void PrintGui(GuiElement* gui, int depth)
{
  std::string spaces(4 * depth, ' ');
  //std::string type = typeid(gui).name();
  std::cout << spaces << gui->GetName(); // << " (" << type << ")";
  GuiComposite* comp = dynamic_cast<GuiComposite*>(gui);
  if (comp)
  {
    int n = comp->GetNumChildren();

    std::cout << ": " << n << (n > 1 ? " children" : " child") << ":\n";

    for (int i = 0; i < n; i++)
    {
      PrintGui(comp->GetChild(i), depth + 1);
    }
  }
  else
  {
    std::cout << "\n";
  }
}

}

