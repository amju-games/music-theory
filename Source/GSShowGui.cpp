// * Amjula music theory *
// (c) Copyright 2026 Juliet Colman

#include <AmjuGL.h>
#include <CommandLineArgs.h>
#include <DrawRect.h>
#include <GuiComposite.h> 
#include "GSShowGui.h"

#define DEBUG_DRAW_RECTS

namespace Amju
{
static void DrawBoxesRecurse(GuiElement* elem)
{
  DrawRect(elem->CalcRect());

  auto comp = dynamic_cast<GuiComposite*>(elem);
  if (comp)
  {
    for (int i = 0; i < comp->GetNumChildren(); i++)
    {   
      DrawBoxesRecurse(comp->GetChild(i));
    }   
  }
}

static void DrawBoxes(GuiElement* elem)
{
#ifdef DEBUG_DRAW_RECTS
  PushColour();
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::SetColour(Colour(0, 1, 0, 1)); 
  AmjuGL::UseShader(nullptr);
  DrawBoxesRecurse(elem);
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  PopColour();
#endif
}

GSShowGui::GSShowGui()
{
  const auto& args = GetCommandLineArgs();
  
  // Get the final command line arg string and use it as gui filename
  const auto& strings = args.GetArgs();
  if (!strings.empty())
  {   
    m_guiFilename = strings.back();
  }   
}

void GSShowGui::Draw2d()
{
  DrawBoxes(GetGui());
  GSBase::Draw2d();
}
}

