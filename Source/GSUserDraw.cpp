// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "GSUserDraw.h"
#include "GuiLineDrawing.h"

namespace Amju
{
void GSUserDraw::Update() 
{
}

void GSUserDraw::OnActive() 
{
  m_gui = LoadGui("Gui/gs_line_drawing.txt");
  m_drawing = dynamic_cast<GuiLineDrawing*>(GetElementByName(m_gui, "line-drawing-test-1"));
}

bool GSUserDraw::OnCursorEvent(const CursorEvent& ce) 
{
  if (m_isDrawing)
  {
    m_drawing->AddPoint(Vec2f(ce.x, ce.y));
    return true;
  }
  return false;
}

bool GSUserDraw::OnMouseButtonEvent(const MouseButtonEvent& mbe) 
{
  m_isDrawing = mbe.isDown;
  return true;
}

bool GSUserDraw::OnDoubleClickEvent(const DoubleClickEvent& dce) 
{
  return false;
}
}