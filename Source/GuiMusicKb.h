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

  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

private:
  struct Key
  {
    // Unique ID
    std::string m_name; // A1, A#1 etc

    // A#, Bb for B flat. Vector of strings for enharmonic names. First is the canonical name, e.g. F, not E#
    Strings m_displayNames;

    int m_midiNote = 0; // note value to play when pressed
    float m_angle = 0; // keys rotate when pressed: angle should be between 0 and 5 (degrees)
    Colour m_colour; // highlighted etc
    PObjMesh m_mesh;
    float m_x = 0; // x-position

    bool LoadFromString(const std::string& s);
  };

  // 3D keys arranged from lowest to highest
  std::vector<Key> m_keys;
};
}


