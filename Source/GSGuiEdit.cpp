// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuGL.h>
#include <DrawRect.h>
#include <GuiComposite.h>
#include "GSGuiEdit.h"
#include "UseVertexColourShader.h"

namespace Amju
{
static void DrawBoundingRects(GuiElement* e)
{
  if (e)
  {
    AmjuGL::SetIdentity();
    Rect r = GetRect(e);
    PushColour();
    AmjuGL::SetColour(Colour(1, 0, 0, 1));
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
    DrawSolidRect(r);
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
    PopColour();

    GuiComposite* comp = dynamic_cast<GuiComposite*>(e);
    if (comp)
    {
      int n = comp->GetNumChildren();
      for (int i = 0; i < n; i++)
      {
        DrawBoundingRects(comp->GetChild(i));
      }
    }
  }
}

GSGuiEdit::GSGuiEdit()
{
  // The GUI for edit mode, e.g. could be a tree view, a palette of
  //  element types, a property box, etc. This is not the GUI that
  //  is being edited - that's m_editGuiFilename.
  m_guiFilename = "Gui/gs_edit_gui.txt"; 
}

void GSGuiEdit::OnActive() 
{
  GSBase::OnActive();

  // Load GUI to edit. 
  if (m_editGuiFilename.empty())
  {
    std::cout << "You have to set the edit GUI filename before activating this state.\n";
    Assert(false);
  }

  m_editGui = LoadGui(m_editGuiFilename);
  if (!m_editGui)
  {
    std::cout << "Failed to load GUI we want to edit: " << m_editGuiFilename << "\n";
    Assert(false);
  }

  std::cout << "Now in GUI Edit mode!\n";
}

void GSGuiEdit::Update()
{
  GSBase::Update();

  if (m_editGui)
  {
    m_editGui->Update();
  }
}

void GSGuiEdit::Draw2d()
{
  GSBase::Draw2d();

  // Draw edit gui, scaled down a bit
  if (m_editGui)
  {
    // Recursively draw bounding rects
    DrawBoundingRects(m_editGui);

    UseVertexColourShader();
    AmjuGL::PushMatrix();
    const float SCALE = 0.8f;
    AmjuGL::Scale(SCALE, SCALE, 1.f);
    m_editGui->Draw();
    AmjuGL::PopMatrix();
  }
}

bool GSGuiEdit::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  // Find out if we have clicked down on a GUI element.
  // If we clicked down, that element becomes active. 
  // What about overlapping elements? We need a z order.
  // We can use their position in the tree in depth first search order.

  return false;
}

bool GSGuiEdit::OnCursorEvent(const CursorEvent& ce) 
{
  return false;
}

void GSGuiEdit::SetEditGuiFilename(const std::string& guifilename)
{
  m_editGuiFilename = guifilename;
}
}

