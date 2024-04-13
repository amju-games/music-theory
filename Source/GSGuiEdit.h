// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "GSBase.h"

namespace Amju
{
// * GSGuiEdit *
// Gui edit mode. 
// TODO Move to amjulib when done and is game-agnostic.
class GSGuiEdit : public GSBase
{
public:
  GSGuiEdit();
  void OnActive() override;
  void Update() override;
  void Draw2d() override;
  bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  bool OnCursorEvent(const CursorEvent&) override;
  bool OnKeyEvent(const KeyEvent&) override;

  // Set the filename for the GUI we will be editing
  void SetEditGuiFilename(const std::string& guifilename);

private:
  void SetSelectedElement(PGuiElement e);

private:
  // Filename of GUI we will edit; not to be confused with the GUI for the
  //  edit mode itself, which we are not editing!
  std::string m_editGuiFilename; 
  PGuiElement m_editGui; // Root node of GUI tree we are editing
  PGuiElement m_selectedElement; // Element we are editing in the tree... we might not need this
  RCPtr<GuiEdit> m_editor; // Editor node, which lets us edit the selected element
};

// Sticking with Singletons; that's the pattern in all this code.
using TheGSGuiEdit = Singleton<GSGuiEdit>;
}

