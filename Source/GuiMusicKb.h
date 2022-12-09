// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <GuiElement.h>
#include <ObjMesh.h>

namespace Amju
{
class GuiMusicKb : public GuiElement
{
public:
  static void AddToFactory();

  virtual void Draw() override;
  virtual void Update() override;
  virtual bool Load(File*) override;

  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  virtual bool OnCursorEvent(const CursorEvent&) override;

private:

  struct Key : public RefCounted
  {
    // Unique ID
    std::string m_name; // A1, A#1 etc

    // A#, Bb for B flat. Vector of strings for enharmonic names. First is the canonical name, e.g. F, not E#
    Strings m_displayNames;

    int m_midiNote = 0; // note value to play when pressed
    float m_angle = 0; // keys rotate when pressed: angle should be between 0 and 5 (degrees)
    float m_desiredAngle = 0;
    Colour m_colour; // for black, white, and also highlighted etc
    PObjMesh m_mesh;
    float m_x = 0; // x-position relative to key to the left
    Rect m_projectedRect; // Rectangle enclosing the projection of the AABB into screen space
    bool m_isBlack = false; // black key: wins in picking

    bool LoadFromString(const std::string& s);
    void CalcRect();
  };
  using PKey = RCPtr<Key>;

  PKey PickKey(const Vec2f& pos);

private:

  // 3D keys arranged from lowest to highest
  std::vector<PKey> m_keys;

  float m_x; // position: we can swipe the keyboard left and right
  float m_desiredX; // position we will lock on to
  Vec2f m_tapDownPos; // position when we tapped down
  Vec2f m_vel;
  bool m_tapDown = false; // true when we are pressing within kb area

  int m_octave; // the currently active octave (1 octave is visible in portrait mode)
};
}


