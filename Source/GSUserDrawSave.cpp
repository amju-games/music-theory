// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include "GSUserDrawSave.h"
#include "GuiLineDrawing.h"

namespace Amju
{
bool GSUserDrawSave::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  GSUserDraw::OnMouseButtonEvent(mbe);

  if (!mbe.isDown)
  {
    // Mouse up: save stroke
    std::cout << "Saving points...\n";
    File f;
    if (f.OpenWrite("points.txt"))
    {
      if (m_drawing->SavePoints(&f))
      {
        std::cout << " ..done saving points, OK apparently.\n";
      }
      else
      {
        f.ReportError("Opened file for writing but failed to save points to it.");
      }
    }
    else
    {
      f.ReportError("Failed to open file for writing.\n");
    }
  }

  return true;
}
}

