// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <EventTypes.h>
#include <GuiElement.h>
#include <RCPtr.h>

namespace Amju
{
class GSMainCorridor;

// Modes for corridor game state
class CorridorMode : public RefCounted
{
public:
  virtual ~CorridorMode() = default;
  void SetGameState(GSMainCorridor* gs) { m_gs = gs; }
  virtual void Update() = 0;  // Call this in overridden functions
  virtual void Draw2d(); // Call this in overridden functions
  virtual void OnActive(); // Call this in overridden functions
  virtual void OnDeactive(); // Call this in overridden functions
  virtual bool OnCursorEvent(const CursorEvent&) { return false; }
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) { return false; }
  virtual void Reset() {}

protected:
  GSMainCorridor* m_gs = nullptr;
  PGuiElement m_gui;
  std::string m_guiFilename;
};
}
