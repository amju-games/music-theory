// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "GuiMusicKbBase.h"

namespace Amju
{
// * GuiMusicKb *
// Musical piano keyboard, which responds to touch events and plays notes.
class GuiMusicKb : public GuiMusicKbBase
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  virtual void Draw() override;
  virtual void Update() override;
  virtual bool Load(File*) override;

  // Might not need these in subclasses
//  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;
//  virtual bool OnCursorEvent(const CursorEvent&) override;

  // Release all keys: safety net to make sure no notes playing
//  void ReleaseAllKeys();

  // Using the palette set in SetPalette, apply colours to the given
  //  keys.
//  void ColouriseKeys(std::vector<int> midiNotes) override;

  // TODO Operations on keys: press, release, highlight, etc.
  // Use the unique name for the key? Or perhaps use the midi value as the unique ID?

  struct Key3d : public GuiMusicKbBase::Key
  {
    PObjMesh m_mesh;

    bool LoadFromString(const std::string& s);
    void CalcRect();
    void Press();
    void Release();
  };

  // Get range of midi notes covered by this keyboard
//  int GetMinKey() const;
//  int GetMaxKey() const;

//  int GetMinKeyOnScreen() const;
//  int GetMaxKeyOnScreen() const;

protected:
  // Find key picked by user
  Key* PickKey(const Vec2f& pos) override;
  
  // Play key: not if KB is disabled. If key is nullptr, release all?
//  void PressKey(Key* key) override;

//  void ReleaseKey(Key* key) override;

protected:
  // Total width of keyboard
//  float m_kbWidth = 0.f;
};
}

