#pragma once

#include <RCPtr.h>
#include <GuiComposite.h>

namespace Amju
{
class GuiMusicKbBase;

// * Qwerty Overlay *
// Displays qwerty keys over the piano keyboard. This is so
//  players can play the game using the qwerty keyboard on desktop
//  (with mouse is awkward).
class QwertyOverlay
{
public:
  // GUI should be included as part of the main state GUI. 
  // (We don't want the qwerty overlays to move when keys are pressed,
  //  so the qwerty keys are not attached to each key.)
  // Here we get a weak pointer to the qwerty keys root.
  //  (Maybe store weak ptr to each key?)
  bool Init(WeakPtr<GuiElement> guiRoot);
  
  // Position the qwerty keys centrally above each piano key.
  bool SetKeyPositions(GuiMusicKbBase& pianoKeys);

  // Returns the qwerty key gui element for the given midi note,
  //  or loads and adds if it doesn't already exist.
  WeakPtr<GuiElement> GetQwertyKeyForMidi(int midi);

  // Get the key text for a qwerty key for the given midi note.
  std::string GetQwertyStrForMidi(int midi) const;

  // Set the key text on the given qwerty key for the given
  //  midi note.
  void SetQwertyStrForMidi(WeakPtr<GuiElement> qwertyKey, int midi);

  void SetVisible(bool visible);

  // Zero out weak ptr
  void Clear();

private:
  // Points to the root node in the gui tree, to which we add 
  //  qwerty key children.
  WeakPtr<GuiComposite> m_qwertyRoot;
};
}
