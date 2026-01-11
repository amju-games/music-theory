// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <GuiComposite.h>
#include <ObjMesh.h>
#include "Palette.h"

namespace Amju
{
// * GuiMusicKb *
// Musical piano keyboard, which responds to touch events and plays notes.
class GuiMusicKb : public GuiComposite
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  // Make sure all keys which were pressed send final key up events
  virtual ~GuiMusicKb();

  virtual void Draw() override;
  virtual void Update() override;
  virtual bool Load(File*) override;

  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  virtual bool OnCursorEvent(const CursorEvent&) override;

  // Release all keys: safety net to make sure no notes playing
  void ReleaseAllKeys();

  // Store palette and apply the contents to the keys.
  // The "colour names" for this palette are MIDI pitch values, e.g.
  //  60=ff0000
  // sets middle C to red.
  void SetPalette(RCPtr<Palette> palette);

  // TODO Operations on keys: press, release, highlight, etc.
  // Use the unique name for the key? Or perhaps use the midi value as the unique ID?

  struct Key : public RefCounted
  {
    // Unique ID. E.g. A1, A#1 etc
    // Any need for this? The midi value is also unique and is a nice int.
    std::string m_name;

    // Name displayed to user. A#, Bb for B flat?
    // Vector of strings to allow for enharmonic names. First is the canonical name, e.g. F, not E#
    Strings m_displayNames;

    int m_midiNote = 0; // note value to play when pressed
    float m_angle = 0; // keys rotate when pressed: angle should be between 0 and 5 (degrees)
    float m_desiredAngle = 0;
    Colour m_colour; // for black, white, and also highlighted etc
    Colour m_naturalColour; // m_colour restores to this colour
    PObjMesh m_mesh;
    float m_x = 0; // x-position relative to key to the left
    Rect m_projectedRect; // Rectangle enclosing the projection of the AABB into screen space
    bool m_isBlack = false; // black key: wins in picking
    bool m_isPressed = false; // true if currently held down

    bool LoadFromString(const std::string& s);
    void CalcRect();
    void Press();
    void Release();
  };
  using PKey = RCPtr<Key>;

  // Get key: midi note is unique ID
  PKey GetKey(int midiNote);

  // Get range of midi notes covered by this keyboard
  int GetMinKey() const;
  int GetMaxKey() const;

  int GetMinKeyOnScreen() const;
  int GetMaxKeyOnScreen() const;

protected:
  // Find key picked by user
  PKey PickKey(const Vec2f& pos);
  
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
    PKey m_key;
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
  int CountFingersOnKey(PKey key) const;

protected:
  // Total width of keyboard
  float m_kbWidth = 0.f;

  // 3D keys arranged from lowest to highest
  std::vector<PKey> m_keys;

  float m_desiredX; // position we will lock on to
  Vec2f m_tapDownPos; // position when we tapped down
  Vec2f m_vel; // swipe velocity
  bool m_tapDown = false; // true when we are touching within kb area
  bool m_tapDownScroll = false; // true when we have touched down to drag KB left or right
  
  PKey m_lastKey; // last key pressed

  // After a Draw call, these are the min and max MIDI notes of the keys on screen. 
  int m_onScreenMin = -1;
  int m_onScreenMax = -1;
};
}


