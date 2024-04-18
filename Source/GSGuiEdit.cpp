// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuGL.h>
#include <DrawBorder.h>
#include <DrawRect.h>
#include <GuiComposite.h>
#include <GuiDialog.h>
#include <GuiEdit.h>
#include <GuiFactory.h>
#include <GuiMenu.h>
#include "GSGuiEdit.h"
#include "UseVertexColourShader.h"

namespace Amju
{
class GuiItemDuplicateCommand : public GuiCommand
{
public:
  GuiItemDuplicateCommand(GuiElement* elem) : m_element(elem) {}

  bool Do() override
  {
    Assert(m_element);
    GuiComposite* comp = dynamic_cast<GuiComposite*>(m_element->GetParent());
    if (!comp)
    {
      return false;
    }

    int index = comp->FindChildIndex(m_element);
    Assert(index != -1);
    PGuiElement clone = m_element->Clone();
    const Vec2f OFFSET{ .05f, -.05f };
    clone->SetLocalPos(clone->GetLocalPos() + OFFSET);
    comp->InsertChild(index + 1, clone);
    m_cloneIndex = index + 1;
    return true;
  }

  void Undo() override
  {
    Assert(m_element);
    GuiComposite* comp = dynamic_cast<GuiComposite*>(m_element->GetParent());
    Assert(comp);
    comp->DeleteChild(m_cloneIndex);
  }

private:
  PGuiElement m_element;
  int m_cloneIndex;
};

class GuiItemForwardBackCommand : public GuiCommand
{
public:
  GuiItemForwardBackCommand(GuiElement* elem, bool upNotDown) : m_element(elem), m_forwardNotBack(upNotDown) {}

  bool Do() override
  {
    Assert(m_element);
    GuiComposite* comp = dynamic_cast<GuiComposite*>(m_element->GetParent());
    if (!comp)
    {
      return false;
    }

    int n = comp->GetNumChildren();
    int index = comp->FindChildIndex(m_element);
    Assert(index != -1);
    if ((m_forwardNotBack && index == n - 1) ||
      (!m_forwardNotBack && index == 0))
    {
      return false;
    }

    comp->DeleteChild(index);
    comp->InsertChild(index + (m_forwardNotBack ? 1 : -1), m_element);
    return true;
  }

  void Undo() override
  {
    m_forwardNotBack = !m_forwardNotBack;
    bool b = Do();
    Assert(b);
    m_forwardNotBack = !m_forwardNotBack;
  }

private:
  PGuiElement m_element;
  bool m_forwardNotBack;
};

class GuiItemDecorateCommand : public GuiCommand
{
public:
  GuiItemDecorateCommand(GuiElement* nodeToDecorate, GuiDecorator* newDecorator) : m_nodeToDecorate(nodeToDecorate), m_newDecorator(newDecorator) {}

  bool Do() override
  {
    if (!m_nodeToDecorate->GetParent())
    {
      return false;
    }
    GuiComposite* compParent = dynamic_cast<GuiComposite*>(m_nodeToDecorate->GetParent());
    if (!compParent)
    {
      return false;
    }
    int index = compParent->FindChildIndex(m_nodeToDecorate);
    Assert(index != -1);
    compParent->InsertChild(index, m_newDecorator);
    compParent->DeleteChild(index + 1);
    m_newDecorator->AddChild(m_nodeToDecorate);
    return true;
  }

  void Undo() override
  {
    GuiComposite* compParent = dynamic_cast<GuiComposite*>(m_newDecorator->GetParent());
    Assert(compParent);
    int index = compParent->FindChildIndex(m_newDecorator);
    Assert(index != -1);
    compParent->InsertChild(index, m_nodeToDecorate);
    compParent->DeleteChild(index + 1);
  }

private:
  PGuiElement m_nodeToDecorate;
  RCPtr<GuiDecorator> m_newDecorator;
};


class GuiItemNewCommand : public GuiCommand
{
public:
  GuiItemNewCommand(GuiComposite* parent, GuiElement* newChild) : m_parent(parent), m_newChild(newChild) {}

  bool Do() override
  {
    m_parent->AddChild(m_newChild);
    return true;
  }

  void Undo() override
  {
    int i = m_parent->FindChildIndex(m_newChild);
    Assert(i != -1);
    m_parent->DeleteChild(i);
  }

private:
  RCPtr<GuiComposite> m_parent;
  PGuiElement m_newChild;
};

class GuiItemDeleteCommand : public GuiCommand
{
public:
  GuiItemDeleteCommand(GuiElement* toDelete) : m_toDelete(toDelete) {}

  bool Do() override
  {
    Assert(m_toDelete);
    GuiElement* parent = m_toDelete->GetParent();
    Assert(parent); // Deleting the root node needs special handling

    GuiComposite* comp = dynamic_cast<GuiComposite*>(parent);
    if (comp)
    {
      // Find the child index so we can put it back where it was.
      m_indexOfDeletedChild = comp->FindChildIndex(m_toDelete);
      Assert(m_indexOfDeletedChild != -1);
      comp->DeleteChild(m_indexOfDeletedChild);
      m_parent = comp;
      // TODO If comp now has no children, should we also delete it? (create a second Undoable command)
      return true; // => undoable
    }
    return false; // => can't undo
  }

  void Undo() override
  {
    m_parent->InsertChild(m_indexOfDeletedChild, m_toDelete);
  }

private:
  PGuiElement m_toDelete;
  RCPtr<GuiComposite> m_parent;
  int m_indexOfDeletedChild = -1;
};

static void UniqueNameForNewElement(PGuiElement elem)
{
  std::string guiTypeName = elem->GetTypeName();
  static std::map<std::string, int> uniqueIds;
  int id = ++uniqueIds[guiTypeName];
  elem->SetName(guiTypeName + "-" + ToString(id));
}

// Convenience function: from a menu item which displays the name of a GUI element,
//  find the named element in the given GUI tree.
static GuiElement* FindGuiElementFromMenuItem(GuiElement* menuItem, GuiElement* guiTreeToSearch)
{
  GuiText* t = dynamic_cast<GuiText*>(menuItem);
  if (t)
  {
    std::string str = t->GetText();
    str = Trim(str);
    return guiTreeToSearch->GetElementByName(str);
  }
  return nullptr;
}

static void OnGuiItemDuplicate(GuiElement* menuItem)
{
  GSGuiEdit* s = dynamic_cast<GSGuiEdit*>(TheGame::Instance()->GetState());
  if (s)
  {
    s->OnGuiItemDuplicate(menuItem);
  }
}

static void OnGuiItemDecorate(GuiElement* menuItem)
{
  GSGuiEdit* s = dynamic_cast<GSGuiEdit*>(TheGame::Instance()->GetState());
  if (s)
  {
    s->OnGuiItemDecorate(menuItem);
  }
}

static void OnGuiItemDelete(GuiElement* menuItem)
{
  GSGuiEdit* s = dynamic_cast<GSGuiEdit*>(TheGame::Instance()->GetState());
  if (s)
  {
    s->OnGuiItemDelete(menuItem);
  }
}

static void OnGuiItemNew(GuiElement* e)
{
  GSGuiEdit* s = dynamic_cast<GSGuiEdit*>(TheGame::Instance()->GetState());
  if (s)
  {
    s->OnGuiItemNew(e);
  }
}

static void OnGuiItemMoveUp(GuiElement* e)
{
  GSGuiEdit* s = dynamic_cast<GSGuiEdit*>(TheGame::Instance()->GetState());
  if (s)
  {
    s->OnGuiItemMoveUp(e);
  }
}

static void OnGuiItemMoveDown(GuiElement* e)
{
  GSGuiEdit* s = dynamic_cast<GSGuiEdit*>(TheGame::Instance()->GetState());
  if (s)
  {
    s->OnGuiItemMoveDown(e);
  }
}

static void OnGuiTreeItemRightClick(GuiElement* e)
{
  GSGuiEdit* s = dynamic_cast<GSGuiEdit*>(TheGame::Instance()->GetState());
  if (s)
  {
    s->OnGuiTreeItemRightClick(e);
  }
}

static void OnGuiTreeItemLeftClick(GuiElement* e)
{
  GSGuiEdit* s = dynamic_cast<GSGuiEdit*>(TheGame::Instance()->GetState());
  if (s)
  {
    s->OnGuiTreeItemLeftClick(e);
  }
}

static void OnUndo(GuiElement*)
{
  GSGuiEdit* s = dynamic_cast<GSGuiEdit*>(TheGame::Instance()->GetState());
  if (s)
  {
    s->OnUndo();
  }
}

static void OnRedo(GuiElement*)
{
  GSGuiEdit* s = dynamic_cast<GSGuiEdit*>(TheGame::Instance()->GetState());
  if (s)
  {
    s->OnRedo();
  }
}

static void OnSaveGui(GuiElement*)
{
  GSGuiEdit* e = dynamic_cast<GSGuiEdit*>(TheGame::Instance()->GetState());
  if (e)
  {
    e->OnSaveGui();
  }
}

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

void GSGuiEdit::OnUndo()
{
  if (TheGuiCommandHandler::Instance()->CanUndo())
  {
    TheGuiCommandHandler::Instance()->Undo();
    m_selectedElement = nullptr;
    m_editor = nullptr;
    PopulateTreeView();
  }
}

void GSGuiEdit::OnRedo()
{
  if (TheGuiCommandHandler::Instance()->CanRedo())
  {
    TheGuiCommandHandler::Instance()->Redo();
    m_selectedElement = nullptr;
    m_editor = nullptr;
    PopulateTreeView();
  }
}

void GSGuiEdit::OnGuiItemDuplicate([[maybe_unused]] GuiElement* menuItem)
{
  if (m_selectedElement)
  {
    TheGuiCommandHandler::Instance()->DoNewCommand(new GuiItemDuplicateCommand(m_selectedElement));
    m_selectedElement = nullptr;
    m_editor = nullptr;
    PopulateTreeView();
  }
}

void GSGuiEdit::OnGuiItemDelete([[maybe_unused]] GuiElement* menuItem)
{
  if (m_selectedElement)
  {
    TheGuiCommandHandler::Instance()->DoNewCommand(new GuiItemDeleteCommand(m_selectedElement));
    m_selectedElement = nullptr;
    m_editor = nullptr;
    PopulateTreeView();
  }
}

void GSGuiEdit::OnGuiItemDecorate(GuiElement* menuItem)
{
  if (m_selectedElement)
  {
    // Adds a decorator child to the selected element's parent, so it must be a composite.
    // TODO Add a root element to an empty tree.
    // The menu item has the type name of the required new child.
    if (m_selectedElement->GetParent())
    {
      GuiComposite* compParent = dynamic_cast<GuiComposite*>(m_selectedElement->GetParent());
      if (compParent)
      {
        GuiText* t = dynamic_cast<GuiText*>(menuItem);
        if (t)
        {
          std::string str = Trim(t->GetText());
          RCPtr<GuiDecorator> newDecorator = dynamic_cast<GuiDecorator*>(TheGuiFactory::Instance()->Create(str));
          if (newDecorator)
          {
            UniqueNameForNewElement(newDecorator.GetPtr());
            TheGuiCommandHandler::Instance()->DoNewCommand(new GuiItemDecorateCommand(m_selectedElement, newDecorator));
            m_selectedElement = nullptr;
            m_editor = nullptr;
            PopulateTreeView();
          }
        }
      }
    }
  }
}

void GSGuiEdit::OnGuiItemNew(GuiElement* menuItem)
{
  if (m_selectedElement)
  {
    // New adds a child to the selected element, so it must be a composite.
    // TODO Add a root element to an empty tree.
    // The menu item has the type name of the required new child.
    GuiComposite* comp = dynamic_cast<GuiComposite*>(m_selectedElement.GetPtr());
    if (comp)
    {
      GuiText* t = dynamic_cast<GuiText*>(menuItem);
      if (t)
      {
        std::string str = Trim(t->GetText());
        PGuiElement newChild = TheGuiFactory::Instance()->Create(str);
        if (newChild)
        {
          UniqueNameForNewElement(newChild);
          newChild->CreateEditorDefault();
          TheGuiCommandHandler::Instance()->DoNewCommand(new GuiItemNewCommand(comp, newChild));
          m_selectedElement = nullptr;
          m_editor = nullptr;
          PopulateTreeView();
        }
      }
    }
  }
}

void GSGuiEdit::OnGuiItemMoveUp([[maybe_unused]] GuiElement* menuItem)
{
  if (m_selectedElement)
  {
    TheGuiCommandHandler::Instance()->DoNewCommand(new GuiItemForwardBackCommand(m_selectedElement, true));
    PopulateTreeView();
  }
}

void GSGuiEdit::OnGuiItemMoveDown([[maybe_unused]] GuiElement* menuItem)
{
  if (m_selectedElement)
  {
    TheGuiCommandHandler::Instance()->DoNewCommand(new GuiItemForwardBackCommand(m_selectedElement, false));
    PopulateTreeView();
  }
}

void GSGuiEdit::OnGuiTreeItemRightClick(GuiElement* e)
{
  m_rightClickTreeViewMenu = new GuiFloatingMenu();

  // TODO This deletes node and all descendants. Have another option to just delete the (decorator) node
  m_rightClickTreeViewMenu->AddChild(new GuiMenuItem("Delete", &Amju::OnGuiItemDelete));

  GuiMenu* newItemMenu = new GuiMenu;
  newItemMenu->AddChild(new GuiMenuItem("gui-comp", &Amju::OnGuiItemNew));
  newItemMenu->AddChild(new GuiMenuItem("spline", &Amju::OnGuiItemNew));
  newItemMenu->AddChild(new GuiMenuItem("poly", &Amju::OnGuiItemNew));
  newItemMenu->AddChild(new GuiMenuItem("gui-image", &Amju::OnGuiItemNew));
  newItemMenu->AddChild(new GuiMenuItem("gui-button", &Amju::OnGuiItemNew));
  newItemMenu->AddChild(new GuiMenuItem("gui-text", &Amju::OnGuiItemNew));
  // TODO etc

  m_rightClickTreeViewMenu->AddChild(new GuiNestMenuItem("New", newItemMenu));
  m_rightClickTreeViewMenu->AddChild(new GuiMenuItem("Move forward", &Amju::OnGuiItemMoveUp)); // i.e. higher in child vec, so drawn later
  m_rightClickTreeViewMenu->AddChild(new GuiMenuItem("Move back", &Amju::OnGuiItemMoveDown)); 

  // TODO!!!!!!!
//  m_rightClickTreeViewMenu->AddChild(new GuiMenuItem("Properties", &Amju::OnGuiItemProperties));

  GuiMenu* decorateItemMenu = new GuiMenu;
  decorateItemMenu->AddChild(new GuiMenuItem("animation", &Amju::OnGuiItemDecorate));
  decorateItemMenu->AddChild(new GuiMenuItem("colour", &Amju::OnGuiItemDecorate));
  decorateItemMenu->AddChild(new GuiMenuItem("elastic", &Amju::OnGuiItemDecorate));
  decorateItemMenu->AddChild(new GuiMenuItem("translate", &Amju::OnGuiItemDecorate));
  decorateItemMenu->AddChild(new GuiMenuItem("scale", &Amju::OnGuiItemDecorate));
  decorateItemMenu->AddChild(new GuiMenuItem("rotate", &Amju::OnGuiItemDecorate));
  m_rightClickTreeViewMenu->AddChild(new GuiNestMenuItem("Decorate", decorateItemMenu));

  m_rightClickTreeViewMenu->AddChild(new GuiMenuItem("Duplicate", &Amju::OnGuiItemDuplicate));

  // Position on left or right of parent menu item
  if (e->GetCombinedPos().x + m_treeview->GetSize().x < .5f)
  {
    m_rightClickTreeViewMenu->SetLocalPos(e->GetCombinedPos() + Vec2f(m_treeview->GetSize().x, 0));
  }
  else
  {
    m_rightClickTreeViewMenu->SetLocalPos(e->GetCombinedPos() - Vec2f(m_rightClickTreeViewMenu->GetSize().x, 0));
  }
}

void GSGuiEdit::OnGuiTreeItemLeftClick(GuiElement* e)
{
  GuiElement* selected = FindGuiElementFromMenuItem(e, m_editGui);
  if (selected)
  {
    SetSelectedElement(selected);
  }
}

void GSGuiEdit::OnSaveGui()
{
  File f(false); // false -> don't automatically write version, we want copyright notice to go at the top.
  if (f.OpenWrite("test_gui_save.txt"))
  {
    // TODO Static data to make this reusable
    if (!f.WriteComment("// * Amjula music theory * (c) Copyright 2024 Juliet Colman"))
    {
      Assert(false);
      return;
    }
    f.WriteInteger(1); // version
    m_editGui->Save(&f);
  }
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

  PopulateTreeView();
  m_rightClickMenu = nullptr;

  std::cout << "Now in GUI Edit mode!\n";
}

void PopulateTreeViewRecursive(GuiMenu* m, GuiElement* e, int depth)
{
  GuiMenuItem* item = new GuiMenuItem(std::string(depth * 4, ' ') + e->GetName(), OnGuiTreeItemLeftClick);
  item->SetRightClickCommand(OnGuiTreeItemRightClick);
  m->AddChild(item);

  GuiComposite* comp = dynamic_cast<GuiComposite*>(e);
  if (comp)
  {
    int n = comp->GetNumChildren();
    for (int i = 0; i < n; i++)
    {
      PopulateTreeViewRecursive(m, comp->GetChild(i), depth + 1);
    }
  }
}

void GSGuiEdit::PopulateTreeView()
{
  Vec2f lastPosForRightClickMenu{ -.9f, .9f };
  if (m_treeview)
  {
    lastPosForRightClickMenu = m_treeview->GetLocalPos();
  }

  m_treeview = new GuiDialog();
  m_treeview->SetLocalPos(lastPosForRightClickMenu);
  m_treeview->SetSize({.5f, .5f });
  m_treeview->SetTitle("GUI tree");
  m_treeview->SetBgColour({ .2f, .2f, .2f, 1.f });

  GuiMenu* treemenu = new GuiMenu;

  treemenu->SetIsVertical(true);
  PopulateTreeViewRecursive(treemenu, m_editGui, 0);

  m_treeview->AddChild(treemenu);
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

  AmjuGL::UseShader(nullptr);
  m_treeview->GetChild(0)->SetVisible(true);
  m_treeview->Draw();

  if (m_rightClickTreeViewMenu)
  {
    m_rightClickTreeViewMenu->Draw();
  }

  if (m_rightClickMenu)
  {
    m_rightClickMenu->Draw();
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
  m_selectedElement = e;
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
  if (m_rightClickTreeViewMenu && 
    m_rightClickTreeViewMenu->OnMouseButtonEvent(mbe))
  {
    return true;
  }

  if (m_treeview->OnMouseButtonEvent(mbe))
  {
    return true;
  }

  // Find out if we have clicked down on a GUI element.
  // If we clicked down, that element becomes active. 
  // What about overlapping elements? We need a z order.
  // We can use their position in the tree in depth first search order.

  if (m_rightClickMenu &&
      m_rightClickMenu->OnMouseButtonEvent(mbe))
  {
    return true;
  }

  if (m_editor)
  {
    if (m_editor->OnMouseButtonEvent(mbe))
    {
      // Editor has consumed the event
      return true;
    }
  }

  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT && mbe.isDown)
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
      m_selectedElement = nullptr;
    }
  }
  else if (mbe.button == AMJU_BUTTON_MOUSE_RIGHT && mbe.isDown)
  {
    m_rightClickMenu = new GuiFloatingMenu();
    m_rightClickMenu->AddChild(new GuiMenuItem("Save", &Amju::OnSaveGui));
    m_rightClickMenu->AddChild(new GuiMenuItem("Undo", &Amju::OnUndo));
    m_rightClickMenu->AddChild(new GuiMenuItem("Redo", &Amju::OnRedo));
    m_rightClickMenu->SetLocalPos(Vec2f(mbe.x, mbe.y));
    return true;
  }

  return false;
}

bool GSGuiEdit::OnCursorEvent(const CursorEvent& ce) 
{
  if (m_rightClickTreeViewMenu &&
    m_rightClickTreeViewMenu->OnCursorEvent(ce))
  {
    return true;
  }

  if (m_treeview->OnCursorEvent(ce))
  {
    return true;
  }

  if (m_rightClickMenu &&
      m_rightClickMenu->OnCursorEvent(ce))
  {
    return true;
  }

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

