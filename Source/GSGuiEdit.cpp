// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuGL.h>
#include <Batched.h>
#include <DrawBorder.h>
#include <DrawRect.h>
#include <GuiComposite.h>
#include <GuiDialog.h>
#include <GuiEdit.h>
#include <GuiFactory.h>
#include <GuiMenu.h>
#include <GuiScroll.h>
#include <GuiTextEdit.h>
#include <GuiTreeView.h>
#include "GSGuiEdit.h"
#include "UseVertexColourShader.h"

namespace Amju
{
class GuiItemDuplicateCommand : public GuiCommand
{
public:
  GuiItemDuplicateCommand(GuiElement* elem) : m_element(elem), m_cloneIndex(-1) {}

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
    Assert(m_cloneIndex != -1);
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
      if (m_indexOfDeletedChild == -1)
      {
        return false;
      }
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

static void OnElementUpdated(PGuiElement elem)
{
  GSGuiEdit* s = dynamic_cast<GSGuiEdit*>(TheGame::Instance()->GetState());
  if (s)
  {
    s->OnElementUpdated(elem);
  }
}

// * PropertyChangeCommand *
// Command to change the named property of the given element, using the given menuItem
//  to get the new value.
class PropertyChangeCommand : public GuiCommand
{
public:
  PropertyChangeCommand(const std::string propertyName, const std::string& newValue, PGuiElement elem)
    : m_propertyName(propertyName), m_newValue(newValue), m_elem(elem)
  {
  }

  bool Do() override
  {
    // Store properties for Undo
    m_oldProperties = m_elem->GetProperties();

    if (m_oldProperties.at(m_propertyName) == m_newValue)
    {
      return false; // no change, nothing to Undo
    }

    // Update properties with changed value
    auto newProperties(m_oldProperties);
    newProperties[m_propertyName] = m_newValue;
    m_elem->SetProperties(newProperties);
    
    // Update the tree view etc to reflect new properties
    OnElementUpdated(m_elem);

    return true;
  }

  void Undo() override
  {
    m_elem->SetProperties(m_oldProperties);
    OnElementUpdated(m_elem);
  }

private:
  const std::string m_propertyName;
  const std::string m_newValue;
  PGuiElement m_elem;
  GuiPropertyMap m_oldProperties;
};

static void OnPropertyTextChanged(PGuiElement elem)
{
  GSGuiEdit* s = dynamic_cast<GSGuiEdit*>(TheGame::Instance()->GetState());
  if (s)
  {
    s->OnPropertyTextChanged(elem);
  }
}

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
  std::string str = menuItem->GetName();
  str = Trim(str);
  return guiTreeToSearch->GetElementByName(str);
}

static void OnGuiItemProperties(GuiElement* menuItem)
{
  GSGuiEdit* s = dynamic_cast<GSGuiEdit*>(TheGame::Instance()->GetState());
  if (s)
  {
    s->OnGuiItemProperties(menuItem);
  }
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

static void OnPlayAnims(GuiElement*)
{
  GSGuiEdit* e = dynamic_cast<GSGuiEdit*>(TheGame::Instance()->GetState());
  if (e)
  {
    e->SetUpdateGuiWeAreEditing(true);
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
    else
    {
      // No select comps, right? We shouldn't be able to move them around, because
      //  comps don't save their pos.
      Rect rect = GetRect(e);
      if (rect.IsPointIn(Vec2f(mbe.x, mbe.y)))
      {
        return e;
      }
    }
  }
  return nullptr;
}

static void FindSelectedElements(GuiElement* root, const Rect& rect, std::vector<PGuiElement>& selection)
{
  if (root)
  {
    // Prefer descendant elements to root elements, so mirroring the draw order
    GuiComposite* comp = dynamic_cast<GuiComposite*>(root);
    if (comp)
    {
      int n = comp->GetNumChildren();
      for (int i = 0; i < n; i++)
      {
        FindSelectedElements(comp->GetChild(i), rect, selection);
      }
    }
    else
    {
      // We don't want to be able to multi-select composites.
      Rect r = GetRect(root);
      if (rect.Intersects(r))
      {
        selection.push_back(root);
      }
    }
  }
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

GSGuiEdit::~GSGuiEdit()
{
}

void GSGuiEdit::SetUpdateGuiWeAreEditing(bool yesUpdate)
{
  m_doUpdateEditGui = yesUpdate;
  m_rightClickMenu->SetVisible(false);
}

void GSGuiEdit::OnUndo()
{
  if (TheGuiCommandHandler::Instance()->CanUndo())
  {
    TheGuiCommandHandler::Instance()->Undo();
    m_selectedElements.clear();
    m_editors.clear();
    PopulateTreeView();
    m_rightClickMenu->SetVisible(false);
  }
}

void GSGuiEdit::OnRedo()
{
  if (TheGuiCommandHandler::Instance()->CanRedo())
  {
    TheGuiCommandHandler::Instance()->Redo();
    m_selectedElements.clear();
    m_editors.clear();
    PopulateTreeView();
    m_rightClickMenu->SetVisible(false);
  }
}

void GSGuiEdit::OnGuiItemProperties([[maybe_unused]] GuiElement* menuItem)
{
  // TODO Handle multi-selection?
  //if (m_selectedElements.size() != 1)
  //{
  //  return;
  //}
  //// Create properties menu
  //m_propertiesMenu = new GuiMenu;
  //PopulatePropertiesMenu(m_propertiesMenu);
  //if (m_propertiesMenuParent)
  //{
  //  m_propertiesMenuParent->AddChild(m_propertiesMenu);
  //}
}

void GSGuiEdit::OnGuiItemDuplicate([[maybe_unused]] GuiElement* menuItem)
{
  if (m_selectedElements.size() == 1)
  {
    TheGuiCommandHandler::Instance()->DoNewCommand(new GuiItemDuplicateCommand(m_selectedElements[0]));
    m_selectedElements.clear();
    m_editors.clear();
    PopulateTreeView();
  }
}

void GSGuiEdit::OnGuiItemDelete([[maybe_unused]] GuiElement* menuItem)
{
  if (m_selectedElements.size() == 1)
  {
    TheGuiCommandHandler::Instance()->DoNewCommand(new GuiItemDeleteCommand(m_selectedElements[0]));
    m_selectedElements.clear();
    m_editors.clear();
    PopulateTreeView();
  }
}

void GSGuiEdit::OnGuiItemDecorate(GuiElement* menuItem)
{
  if (m_selectedElements.size() == 1)
  {
    // Adds a decorator child to the selected element's parent, so it must be a composite.
    // TODO Add a root element to an empty tree.
    // The menu item has the type name of the required new child.
    if (m_selectedElements[0]->GetParent())
    {
      GuiComposite* compParent = dynamic_cast<GuiComposite*>(m_selectedElements[0]->GetParent());
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
            TheGuiCommandHandler::Instance()->DoNewCommand(new GuiItemDecorateCommand(m_selectedElements[0], newDecorator));
            m_selectedElements.clear();
            m_editors.clear();
            PopulateTreeView();
          }
        }
      }
    }
  }
}

void GSGuiEdit::OnGuiItemNew(GuiElement* menuItem)
{
  if (m_selectedElements.size() == 1)
  {
    // New adds a child to the selected element, so it must be a composite.
    // TODO Add a root element to an empty tree.
    // The menu item has the type name of the required new child.
    GuiComposite* comp = dynamic_cast<GuiComposite*>(m_selectedElements[0].GetPtr());
    if (comp)
    {
      GuiTextMenuItem* gtmi = dynamic_cast<GuiTextMenuItem*>(menuItem);
      if (gtmi)
      {
        GuiText* t = dynamic_cast<GuiText*>(gtmi->GetChild());
        if (t)
        {
          std::string str = Trim(t->GetText());
          PGuiElement newChild = TheGuiFactory::Instance()->Create(str);
          if (newChild)
          {
            UniqueNameForNewElement(newChild);
            newChild->CreateEditorDefault();
            TheGuiCommandHandler::Instance()->DoNewCommand(new GuiItemNewCommand(comp, newChild));
            m_selectedElements.clear();
            m_editors.clear();
            PopulateTreeView();
          }
        }
      }
    }
  }
}

void GSGuiEdit::OnGuiItemMoveUp([[maybe_unused]] GuiElement* menuItem)
{
  for (auto& e : m_selectedElements)
  {
    TheGuiCommandHandler::Instance()->DoNewCommand(new GuiItemForwardBackCommand(e, true));
    PopulateTreeView();
  }
}

void GSGuiEdit::OnGuiItemMoveDown([[maybe_unused]] GuiElement* menuItem)
{
  for (auto& e : m_selectedElements)
  {
    TheGuiCommandHandler::Instance()->DoNewCommand(new GuiItemForwardBackCommand(e, false));
    PopulateTreeView();
  }
}

void GSGuiEdit::OnElementUpdated(GuiElement* e)
{
  PopulateTreeView();
}

void GSGuiEdit::AddNewItemMenuToRightClickTreeView()
{
  GuiMenu* newItemMenu = new GuiMenu;
  newItemMenu->AddChild(new GuiTextMenuItem("gui-comp", &Amju::OnGuiItemNew));
  newItemMenu->AddChild(new GuiTextMenuItem("spline", &Amju::OnGuiItemNew));
  newItemMenu->AddChild(new GuiTextMenuItem("poly", &Amju::OnGuiItemNew));
  newItemMenu->AddChild(new GuiTextMenuItem("gui-image", &Amju::OnGuiItemNew));
  newItemMenu->AddChild(new GuiTextMenuItem("gui-button", &Amju::OnGuiItemNew));
  newItemMenu->AddChild(new GuiTextMenuItem("gui-text", &Amju::OnGuiItemNew));
  // TODO etc
  m_rightClickTreeViewMenu->AddChild(new GuiNestMenuItem("New >", newItemMenu));
}

void GSGuiEdit::PopulatePropertiesMenu(GuiMenu* menu)
{
  Assert(m_selectedElements.size() == 1);
  auto selectedElement = m_selectedElements[0];
  auto properties = selectedElement->GetProperties();

  // Create a menu, add each property as an editable menu item
  for (auto& [name, value] : properties)
  {
    // Create an editor menu item (TODO according to the type of the property);
    //  populate it with initial value
    GuiMenu* container = new GuiMenu; // contains one item: the property editor item
    container->SetName(name); // set name=>property name so we can retrieve it in OnPropertyTextChanged

    GuiTextEditMenuItem* textEditMenuItem = new GuiTextEditMenuItem(value);
    textEditMenuItem->SetOnTextChangedCommand(&Amju::OnPropertyTextChanged); 
    container->AddChild(textEditMenuItem);

    menu->AddChild(new GuiNestMenuItem(name + " > ", container));
  }
}

void GSGuiEdit::OnPropertyTextChanged(GuiElement* elem)
{
  GuiTextEditMenuItem* textEditMenuItem = dynamic_cast<GuiTextEditMenuItem*>(elem);
  Assert(textEditMenuItem);
  // Parent container menu has name==property name, see PopulatePropertiesMenu()
  GuiElement* parent = textEditMenuItem->GetParent();
  const std::string& propertyName = parent->GetName();
  const std::string& newValue = textEditMenuItem->GetText();

  Assert(m_selectedElements.size() == 1);
  auto selectedElement = m_selectedElements[0];

  parent->SetVisible(false);

  TheGuiCommandHandler::Instance()->DoNewCommand(
    new PropertyChangeCommand(propertyName, newValue, selectedElement));
}

void GSGuiEdit::AddPropertiesMenuToRightClickTreeView()
{
  GuiMenu* propertiesMenu = new GuiMenu;
  propertiesMenu->SetName("properties sub menu");
  PopulatePropertiesMenu(propertiesMenu);
  m_rightClickTreeViewMenu->AddChild(new GuiNestMenuItem("Properties >", propertiesMenu));
}

void GSGuiEdit::AddDecorateMenuToRightClickTreeView()
{
  GuiMenu* decorateItemMenu = new GuiMenu;
  decorateItemMenu->AddChild(new GuiTextMenuItem("animation", &Amju::OnGuiItemDecorate));
  decorateItemMenu->AddChild(new GuiTextMenuItem("colour", &Amju::OnGuiItemDecorate));
  decorateItemMenu->AddChild(new GuiTextMenuItem("elastic", &Amju::OnGuiItemDecorate));
  decorateItemMenu->AddChild(new GuiTextMenuItem("translate", &Amju::OnGuiItemDecorate));
  decorateItemMenu->AddChild(new GuiTextMenuItem("scale", &Amju::OnGuiItemDecorate));
  decorateItemMenu->AddChild(new GuiTextMenuItem("rotate", &Amju::OnGuiItemDecorate));
  m_rightClickTreeViewMenu->AddChild(new GuiNestMenuItem("Decorate >", decorateItemMenu));
}

void GSGuiEdit::PopulateRightClickTreeViewMenu()
{
  AddPropertiesMenuToRightClickTreeView();

  m_rightClickTreeViewMenu->AddChild(new GuiTextMenuItem("Duplicate", &Amju::OnGuiItemDuplicate));
  AddDecorateMenuToRightClickTreeView();
  AddNewItemMenuToRightClickTreeView();

  m_rightClickTreeViewMenu->AddChild(new GuiTextMenuItem("Bring forward", &Amju::OnGuiItemMoveUp));
  m_rightClickTreeViewMenu->AddChild(new GuiTextMenuItem("Send back", &Amju::OnGuiItemMoveDown));

  // TODO This deletes node and all descendants. Have another option to just delete the (decorator) node
  m_rightClickTreeViewMenu->AddChild(new GuiTextMenuItem("Delete", &Amju::OnGuiItemDelete));
}

void GSGuiEdit::OnGuiTreeItemRightClick(GuiElement* e)
{
  // If nothing is selected, select the menu item under the cursor.
  if (m_selectedElements.empty())
  {
    OnGuiTreeItemLeftClick(e);
  }
  Assert(!m_selectedElements.empty());

  m_rightClickTreeViewMenu = new GuiFloatingMenu();
  PopulateRightClickTreeViewMenu();

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

void GSGuiEdit::HideRightClickMenu()
{
  m_rightClickMenu = nullptr;
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

    m_rightClickMenu->SetVisible(false);
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

  m_selectionRectIsActive = false;
  SetSelectedElement(nullptr);

  PopulateTreeView();
  m_rightClickMenu = nullptr;

  std::cout << "Now in GUI Edit mode!\n";
}

static void PopulateTreeViewRecursive(GuiTreeView* treeview, GuiElement* e, int depth)
{
  // TODO TreeViewItem
  GuiTextMenuItem* item = new GuiTextMenuItem(std::string(depth * 4, ' ') + e->GetName(), OnGuiTreeItemLeftClick);
  // TODO Also set right click command, so we select this element if we right click its 
  //  name in the menu/tree.


  item->SetRightClickCommand(OnGuiTreeItemRightClick);
  treeview->AddChild(item);

  GuiComposite* comp = dynamic_cast<GuiComposite*>(e);
  if (comp)
  {
    int n = comp->GetNumChildren();
    for (int i = 0; i < n; i++)
    {
      PopulateTreeViewRecursive(treeview, comp->GetChild(i), depth + 1);
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
  m_treeview->SetSize({ .5f, .5f });
  m_treeview->SetTitle("GUI tree");
  m_treeview->SetBgColour({ .2f, .2f, .2f, 1.f });

  GuiTreeView* treemenu = new GuiTreeView;

  treemenu->SetIsVertical(true);
  PopulateTreeViewRecursive(treemenu, m_editGui, 0);

  GuiScroll* scrollbar = new GuiScroll;
  m_treeview->AddChild(scrollbar);
  treemenu->SetLocalPos(Vec2f(0, 0));
  scrollbar->SetSize(Vec2f(.1f, 1.f));
  scrollbar->AddChild(treemenu);
  float h = treemenu->GetSize().y;
  scrollbar->SetExtents(Vec2f(0, h));
  //scrollbar->InitScrollBar();
}

void GSGuiEdit::Update()
{
  GSBase::Update();

  if (m_editGui &&
    m_doUpdateEditGui)
  {
    m_editGui->Update();
  }

  if (m_treeview)
  {
    m_treeview->Update();
  }
}

void GSGuiEdit::SetZoomMatricesFromModelview()
{
  m_zoomMatrix.ModelView();
  bool inverseOk = m_zoomMatrix.Inverse(&m_inverseZoomMatrix);
  Assert(inverseOk); // zoom matrix should always be invertible, right?!
}

MouseButtonEvent GSGuiEdit::ZoomTransform(const MouseButtonEvent& mbe) const
{
  Vec3f v(mbe.x, mbe.y, 0);
  Vec3f transformedV = v * m_inverseZoomMatrix;
  MouseButtonEvent result(mbe);
  result.x = transformedV.x;
  result.y = transformedV.y;
  return result;
}

CursorEvent GSGuiEdit::ZoomTransform(const CursorEvent& ce) const
{
  Vec3f v(ce.x, ce.y, 0);
  Vec3f transformedV = v * m_inverseZoomMatrix;
  CursorEvent result(ce);
  result.x = transformedV.x;
  result.y = transformedV.y;
  return result;
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
    AmjuGL::Translate(m_zoomAnchor.x, m_zoomAnchor.y, 0);
    AmjuGL::Scale(m_zoom, m_zoom, 1.f);
    AmjuGL::Translate(-m_zoomAnchor.x, -m_zoomAnchor.y, 0);

    // store transform for drawing editing grabbers etc and transforming mouse coords
    SetZoomMatricesFromModelview();

    m_editGui->Draw();
    Batched::DrawAll(); // flush all polys in GUI we are editing

    // Recursively draw bounding rects and grabbers
    AmjuGL::UseShader(nullptr);
    DrawBoundingRects(m_editGui);

    for (auto& ed : m_editors)
    {
      ed->Draw();
    }

    if (true) // m_selectionRectIsActive) // Hmm what's going on here?
    {
      AmjuGL::UseShader(nullptr);
      AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
      PushColour();
      AmjuGL::SetColour({0, 1, 0, 1});
      DrawRect(m_selectionRect);
      PopColour();
      AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
    }

    AmjuGL::PopMatrix();
  }

  AmjuGL::UseShader(nullptr);
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

void GSGuiEdit::SetElementsInSelectionRect()
{
  m_selectedElements.clear();
  m_editors.clear();

  FindSelectedElements(m_editGui, m_selectionRect, m_selectedElements);

  for (auto& elem : m_selectedElements)
  {
    RCPtr<GuiEdit> editor = elem->CreateEditor();
    m_editors.push_back(editor);
    editor->SetChild(elem); // Not AddChild(), which would set Editor node as parent.
    editor->RecalcGrabberPositions();
    editor->SelectGrabbersInRect(m_selectionRect);
  }
}

void GSGuiEdit::SetSelectedElement(PGuiElement e)
{
  m_selectedElements.clear();
  m_editors.clear();
  if (e)
  {
    m_selectedElements.push_back(e);
    RCPtr<GuiEdit> editor = e->CreateEditor();
    m_editors.push_back(editor);
    editor->SetChild(e); // Not AddChild(), which would set Editor node as parent.
    editor->RecalcGrabberPositions();
  }
}

bool GSGuiEdit::OnMiddleButtonEvent(const MouseButtonEvent& mbe)
{
  Assert(mbe.button == AMJU_BUTTON_MOUSE_MIDDLE);
  m_zoomIsActive = mbe.isDown;
  if (m_zoomIsActive && m_zoom <= 1.f)
  {
    m_zoomAnchor = Vec2f(mbe.x, mbe.y);
  }
  return true;
}

bool GSGuiEdit::OnCursorZoomEvent(const CursorEvent& ce)
{
  Assert(m_zoomIsActive);
  m_zoom += ce.dx;
  if (m_zoom < 1.f)
  {
    m_zoom = 1.f;
  }
  return true;
}

bool GSGuiEdit::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  // Check for middle button, we can use this to zoom.
  if (mbe.button == AMJU_BUTTON_MOUSE_MIDDLE)
  {
    return OnMiddleButtonEvent(mbe);
  }

  // Destroy right click sub-menu of tree view if it is not in use, so it doesn't
  //  continue to consume events.
  // TODO Do the same for the right-click menu (Save/Undo etc)
  // TODO We don't need to do this here. Maybe in Update()?
  if (m_rightClickTreeViewMenu &&
    !m_rightClickTreeViewMenu->IsVisible())
  {
    m_rightClickTreeViewMenu = nullptr;
  }

  if (m_rightClickTreeViewMenu &&
    m_rightClickTreeViewMenu->OnMouseButtonEvent(mbe))
  {
    return true;
  }

  if (m_treeview->OnMouseButtonEvent(mbe))
  {
    return true;
  }

  if (m_rightClickMenu &&
    m_rightClickMenu->OnMouseButtonEvent(mbe))
  {
    return true;
  }

  auto zoomedMbe = ZoomTransform(mbe);

  // Find out if we have clicked down on a GUI element.
  // If we clicked down, that element becomes active. 
  // What about overlapping elements? We need a z order.
  // As there is no explicit z order, we use their position in the tree in depth first search order.
  if (!m_editors.empty())
  {
    bool b = false;
    for (auto& ed : m_editors)
    {
      b |= ed->OnMouseButtonEvent(zoomedMbe);
    }
    if (b)
    {
      // One or more editors consumed the event
      return true;
    }
  }

  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT && mbe.isDown)
  {
    // No corner grabbed: check for a selected element
    GuiElement* selected = FindSelectedElement(m_editGui, zoomedMbe);
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
      m_selectedElements.clear();
      m_editors.clear();
    }
  }
  else if (mbe.button == AMJU_BUTTON_MOUSE_RIGHT && mbe.isDown)
  {
    m_rightClickMenu = new GuiFloatingMenu();
    m_rightClickMenu->AddChild(new GuiTextMenuItem("Save", &Amju::OnSaveGui));

    auto commandHandler = TheGuiCommandHandler::Instance();
    auto undoMenuItem = new GuiTextMenuItem("Undo", &Amju::OnUndo);
    undoMenuItem->SetEnabled(commandHandler->CanUndo());
    m_rightClickMenu->AddChild(undoMenuItem);

    auto redoMenuItem = new GuiTextMenuItem("Redo", &Amju::OnRedo);
    redoMenuItem->SetEnabled(commandHandler->CanRedo());
    m_rightClickMenu->AddChild(redoMenuItem);

    // Play or pause anims, set menu item text according to current state
    std::string playOrPause = (m_doUpdateEditGui ? "Pause anims" : "Play anims");
    m_rightClickMenu->AddChild(new GuiTextMenuItem(playOrPause, &Amju::OnPlayAnims));

    m_rightClickMenu->SetLocalPos(Vec2f(mbe.x, mbe.y));
    return true;
  }

  m_selectionRectIsActive = mbe.isDown;
  if (m_selectionRectIsActive)
  {
    // Use zoomed mbe here, right?
    float x = zoomedMbe.x;
    float y = zoomedMbe.y;
    m_selectionRectAnchor = Vec2f(x, y);
    m_selectionRect.Set(x, x, y, y);
  }
  else
  {
    SetElementsInSelectionRect();
  }

  return false;
}

bool GSGuiEdit::OnCursorEvent(const CursorEvent& ce) 
{
  if (m_zoomIsActive)
  {
    return OnCursorZoomEvent(ce);
  }

  auto zoomedCe = ZoomTransform(ce);

  if (m_selectionRectIsActive)
  {
    float x = zoomedCe.x;
    float y = zoomedCe.y;
    m_selectionRect.Set(m_selectionRectAnchor.x, x, m_selectionRectAnchor.y, y);
  }

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

  if (!m_editors.empty())
  {
    bool b = false;
    for (auto& ed : m_editors)
    {
      b |= ed->OnCursorEvent(zoomedCe);
    }
    if (b)
    {
      return true;
    }
  }

  return false;
}

void GSGuiEdit::SetEditGuiFilename(const std::string& guifilename)
{
  m_editGuiFilename = guifilename;
}
}

