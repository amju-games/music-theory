// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <EventTypes.h>
#include <GuiElement.h>
#include <ObjMesh.h>

namespace Amju
{
struct MusicKbEvent;
struct MusicEventListener
{
  virtual ~MusicEventListener() = default;
  virtual void OnMusicKbEvent(MusicKbEvent*) = 0;
};

// Event fired when we key up or down. Classes derived from MusicEventListener 
//  can get the event.
struct MusicKbEvent : public Event
{
  MusicKbEvent() = default;
  MusicKbEvent(int note, bool on) : m_note(note), m_on(on) {}
  virtual bool UpdateListener(EventListener*) override;
  int m_note = 0;
  bool m_on = true;
};

class GuiMusicKb : public GuiElement
{
public:
  static void AddToFactory();
  
  // Make sure all keys which were pressed send final key up events
  virtual ~GuiMusicKb();

  virtual void Draw() override;
  virtual void Update() override;
  virtual bool Load(File*) override;

  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  virtual bool OnCursorEvent(const CursorEvent&) override;

  // TODO Operations on keys: press, release, highlight, etc.
  // Use the unique name for the key? Or perhaps use the midi value as the unique ID?

private:

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

  // Find key picked by user
  PKey PickKey(const Vec2f& pos);
  
  // Release all keys: good safety net but means KB is monophonic
  void ReleaseAllKeys();

private:

  // 3D keys arranged from lowest to highest
  std::vector<PKey> m_keys;

  float m_x; // position: we can swipe the keyboard left and right
  float m_desiredX; // position we will lock on to
  Vec2f m_tapDownPos; // position when we tapped down
  Vec2f m_vel; // swipe velocity
  bool m_tapDown = false; // true when we are touching within kb area

  int m_octave; // the currently active octave (1 octave is visible in portrait mode)
};
}


