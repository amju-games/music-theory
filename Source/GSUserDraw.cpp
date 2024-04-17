// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <GuiComposite.h>
#include "GSUserDraw.h"
#include "GuiSpline.h"

namespace Amju
{
void GSUserDraw::Update() 
{
}

void GSUserDraw::OnActive() 
{
  m_gui = LoadGui("Gui/gs_user_draw.txt");
  //m_drawing = dynamic_cast<GuiLineDrawing*>(GetElementByName(m_gui, "line-drawing-test-1"));

  // TODO Left handed pencil option
  m_pencil = GetElementByName(m_gui, "pencil");
}

bool GSUserDraw::OnCursorEvent(const CursorEvent& ce) 
{
  // TODO TEMP TEST CONFIG
  // Offset so we can see the pencil tip
  Vec2f pos = Vec2f(ce.x, ce.y) + Vec2f(-0.3f, 0.3f);

  if (m_pencil)
  {
    m_pencil->SetLocalPos(pos);
  }

  if (m_isDrawing && m_drawing)
  {
    //m_drawing->AddPoint(pos);
    return true;
  }
  return false;
}

bool GSUserDraw::OnMouseButtonEvent(const MouseButtonEvent& mbe) 
{
  m_isDrawing = mbe.isDown;

  if (m_isDrawing)
  {
    // Create new stroke object and add to scene root

    m_drawing = new GuiSpline;
    m_drawing->SetOutlineColour(Colour(0, 0, 0, 1));
    GuiComposite* comp = dynamic_cast<GuiComposite*>(GetElementByName(m_gui, "root"));
    comp->AddChild(m_drawing);
//TODO TEMP TEST    comp->BringChildToFront(m_pencil);
  }

  return true;
}

bool GSUserDraw::OnDoubleClickEvent(const DoubleClickEvent& dce) 
{
  return false;
}
}
