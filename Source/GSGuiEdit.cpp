// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuGL.h>
#include <DrawBorder.h>
#include <DrawRect.h>
#include <GuiComposite.h>
#include <GuiEdit.h>
#include "GSGuiEdit.h"
#include "UseVertexColourShader.h"

namespace Amju
{
static GuiElement* FindSelectedElement(GuiElement* e, const MouseButtonEvent& mbe)
{
  if (e)
  {
    // Prefer descendant elements to root elements, so mirroring the draw order
    GuiComposite* comp = dynamic_cast<GuiComposite*>(e);
    if (comp)
    {
      int n = comp->GetNumChildren();
      // Reverse iterate through children, preferring last sibling to first
      for (int i = n - 1; i >= 0; i--)
      {
        GuiElement* selected = FindSelectedElement(comp->GetChild(i), mbe);
        if (selected)
        {
          return selected;
        }
      }
    }

    Rect rect = GetRect(e);
    if (rect.IsPointIn(Vec2f(mbe.x, mbe.y)))
    {
      return e;
    }
  }
  return nullptr;
}

static void DrawBoundingRects(GuiElement* e)
{
  if (e)
  {
    GuiComposite* comp = dynamic_cast<GuiComposite*>(e);
    if (comp)
    {
      int n = comp->GetNumChildren();
      for (int i = 0; i < n; i++)
      {
        DrawBoundingRects(comp->GetChild(i));
      }
      DrawBorder(e, Colour(0, 0, 1.0f, 0.25f), 0);
    }
    else
    {
      //DrawFilled(e, Colour(0, 0.5f, 1.0f, 0.01f), 0.01f);
      DrawBorder(e, Colour(0, 0, 1.0f, 1.0f), 0);
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

  SetSelectedElement(nullptr);

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
//    GuiElement::SetGlobalScale(0.8f); // Problems with this

    UseVertexColourShader();
    AmjuGL::PushMatrix(); // ?
    m_editGui->Draw();
    AmjuGL::PopMatrix(); // ?

    // Recursively draw bounding rects
    AmjuGL::UseShader(nullptr);
    DrawBoundingRects(m_editGui);

    if (m_editor)
    {
      m_editor->Draw();
    }

    //GuiElement::SetGlobalScale(1.0f);
  }
}

bool GSGuiEdit::OnKeyEvent(const KeyEvent& ke)
{
  // TODO check for some keys, but not 'e' because that takes us here! 
  // Probably also not reloading gui, because that could overwrite changes we are making.

  if (CheckForKey_B_BackToPrevState(ke))
  {
    // TODO Wait, we want to prompt to save, no?
    // Or do we save all changes immediately?
    return true;
  }

  return false;
}

void GSGuiEdit::SetSelectedElement(PGuiElement e)
{
  //m_selectedElement = e;
  if (e)
  {
    m_editor = e->CreateEditor();
    m_editor->SetChild(e); // Not AddChild(), which would set Editor node as parent.
    m_editor->RecalcGrabberPositions();
  }
  else
  {
    m_editor = nullptr;
  }
}

bool GSGuiEdit::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  // Find out if we have clicked down on a GUI element.
  // If we clicked down, that element becomes active. 
  // What about overlapping elements? We need a z order.
  // We can use their position in the tree in depth first search order.

  if (m_editor)
  {
    if (m_editor->OnMouseButtonEvent(mbe))
    {
      // Editor has consumed the event
      return true;
    }
  }

  if (mbe.isDown)
  {
    // No corner grabbed: check for a selected element
    GuiElement* selected = FindSelectedElement(m_editGui, mbe);
    //if (dynamic_cast<GuiComposite*>(selected))
    //{
    //  selected = nullptr; // can't select a composite?
    //  //m_selectedElement = nullptr;
    //}
    if (selected)
    {
      SetSelectedElement(selected);
      std::cout << "Selecting: " << selected->GetName() << "\n";
      return true;
    }
    else
    {
      m_editor = nullptr;
    }
  }

  return false;
}

bool GSGuiEdit::OnCursorEvent(const CursorEvent& ce) 
{
  if (m_editor)
  {
    return m_editor->OnCursorEvent(ce);
  }

  return false;
}

void GSGuiEdit::SetEditGuiFilename(const std::string& guifilename)
{
  m_editGuiFilename = guifilename;
}
}

