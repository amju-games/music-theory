// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <GuiElement.h>
#include <ObjMesh.h>
#include "Palette.h"

namespace Amju
{
// * GuiMusicKbBase *
// Base class for 2D and 3D piano keyboards
class GuiMusicKbBase : public GuiElement
{
public:
  // Make sure all keys which were pressed send final key up events
  virtual ~GuiMusicKbBase();

  void Update() override;
  bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  bool OnCursorEvent(const CursorEvent&) override;

  // Release all keys: safety net to make sure no notes playing
  void ReleaseAllKeys();

  // Store palette 
  void SetPalette(RCPtr<Palette> palette);

  // Using the palette set in SetPalette, apply colours to the given
  //  keys.
  // Base class version sets members in m_keys, could be overridden.
  virtual void ColouriseKeys(std::vector<int> midiNotes);

  // TODO Operations on keys: press, release, highlight, etc.
  // Use the unique name for the key? Or perhaps use the midi value as the unique ID?

  struct Key : public RefCounted
  {
    // Unique ID. E.g. A1, A#1 etc
    // Any need for this? The midi value is also unique and is a nice int.
    std::string m_name;

    // Name displayed to user. This is set by score, which decides what
    //  to do about enharmonic names.
    std::string m_displayName;

    int m_midiNote = 0; // note value to play when pressed
    float m_angle = 0; // keys rotate when pressed: angle should be between 0 and 5 (degrees)
    float m_desiredAngle = 0;
    Colour m_colour; // for black, white, and also highlighted etc
    Colour m_naturalColour; // m_colour restores to this colour
    float m_x = 0; // x-position relative to key to the left
    // Rectangle enclosing the projection of the AABB of the key into screen space
    Rect m_projectedRect; 

    bool m_isBlack = false; // black key: wins in picking
    bool m_isPressed = false; // true if currently held down

    virtual void Press();
    virtual void Release();
  };

  // Get key: midi note is unique ID
  Key* GetKey(int midiNote) const;

  // Get range of midi notes covered by this keyboard
  int GetMinKey() const;
  int GetMaxKey() const;

  int GetMinKeyOnScreen() const;
  int GetMaxKeyOnScreen() const;

  // Get screen x-coord of middle of key
  float GetKeyMidX(int midiKey) const;

protected:
  // Find key picked by user; relies on projected rect in Keys.
  Key* PickKey(const Vec2f& pos);
  
  // Play key: not if KB is disabled. If key is nullptr, release all?
  void PressKey(Key* key);

  void ReleaseKey(Key* key);

  // * Finger *
  // Keep track of touch down/move/up events
  struct Finger
  {
    Finger() = default;
    explicit Finger(const Vec2f& pos) : m_pos(pos) {}

    // Location at which we touched down
    // Is this useful?
    Vec2f m_touchDownPos;

    // Most recent position
    Vec2f m_pos;

    // Most recent key we pick - this can change as touch moves
    Key* m_key;
  };
 
  using Fingers = std::vector<Finger>;
  Fingers m_fingers;

  // Touch down event: add a new finger
  void AddFinger(const Vec2f& touchDownPos);
  
  // Touch up event: remove finger
  void EraseClosestFinger(const Vec2f& touchUpPos);

  // Move event: update closest finger
  void MoveClosestFinger(const Vec2f& cursorPos);

  Fingers::iterator FindClosestFinger(const Vec2f& pos);

  // Count the number of fingers on the given key
  int CountFingersOnKey(Key* key) const;

protected:
  // Total width of keyboard
  float m_kbWidth = 0.f;

  // Keys arranged from lowest to highest.
  // TODO Should this be a map - we are binary searching to find the key
  //  for a midi pitch.
  // NO! We can just index into this vector!! -- TODO
  using PKey = RCPtr<Key>;
  std::vector<PKey> m_keys; // Dynamic type depends on 2D/3D keyboard type

  // For dragging keyboard left and right; conflicts with glissando so
  //  probably always disabled.
  float m_desiredX; // position we will lock on to
  Vec2f m_tapDownPos; // position when we tapped down
  Vec2f m_vel; // swipe velocity
  bool m_tapDown = false; // true when we are touching within kb area
  bool m_tapDownScroll = false; // true when we have touched down to drag KB left or right
  
  Key* m_lastKey; // last key pressed

  // After a Draw call, these are the min and max MIDI notes of the keys on screen. 
  int m_onScreenMin = -1;
  int m_onScreenMax = -1;

  RCPtr<Palette> m_palette;
};
}

