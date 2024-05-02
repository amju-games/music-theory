// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "GSBase.h"

namespace Amju
{
class GuiMenu;
class GuiDialog;

// * GSGuiEdit *
// Gui edit mode. 
// TODO Move to amjulib when done and is game-agnostic.
class GSGuiEdit : public GSBase
{
public:
  GSGuiEdit();
  ~GSGuiEdit();
  void OnActive() override;
  void Update() override;
  void Draw2d() override;
  bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  bool OnCursorEvent(const CursorEvent&) override;
  bool OnKeyEvent(const KeyEvent&) override;

  // Set the filename for the GUI we will be editing
  void SetEditGuiFilename(const std::string& guifilename);

  void OnSaveGui();
  void OnGuiTreeItemLeftClick(GuiElement* e);
  void OnGuiTreeItemRightClick(GuiElement* e);
  void OnGuiItemDelete(GuiElement* e);
  void OnGuiItemNew(GuiElement* e);
  void OnGuiItemMoveUp(GuiElement* e);
  void OnGuiItemMoveDown(GuiElement* e);
  void OnGuiItemDecorate(GuiElement* e);
  void OnGuiItemDuplicate(GuiElement* e);
  void OnGuiItemProperties(GuiElement* e);
  void OnUndo();
  void OnRedo();
  void SetUpdateGuiWeAreEditing(bool yesUpdate); // Call to play/pause animations on the GUI we are editing

  void HideRightClickMenu();

private:
  void SetSelectedElement(PGuiElement e);
  void SetElementsInSelectionRect(); // multi-select
  void PopulateTreeView();

private:
  // Filename of GUI we will edit; not to be confused with the GUI for the
  //  edit mode itself, which we are not editing!
  std::string m_editGuiFilename; 
  PGuiElement m_editGui; // Root node of GUI tree we are editing
  //PGuiElement m_selectedElement; // Element we are editing
  std::vector<PGuiElement> m_selectedElements;
  std::vector<RCPtr<GuiEdit>> m_editors; // Editor nodes, which lets us edit the selected elements.
  // Each selected element has an editor pointing to it.

  RCPtr<GuiDialog> m_treeview; // Show GUI tree in vertical text form
  RCPtr<GuiMenu> m_rightClickMenu;
  RCPtr<GuiMenu> m_rightClickTreeViewMenu;

  // By default, don't update the GUI we are editing, to turn off peskly animations.
  bool m_doUpdateEditGui = false;

  Rect m_selectionRect;
  Vec2f m_selectionRectAnchor;
  bool m_selectionRectIsActive = false;
};

// Sticking with Singletons; that's the pattern in all this code.
using TheGSGuiEdit = Singleton<GSGuiEdit>;
}

