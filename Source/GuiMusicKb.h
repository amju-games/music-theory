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

private:
  struct Key
  {
    // Unique ID
    std::string m_name; // A1, A#1 etc

    // A#, Bb for B flat. Vector of strings for enharmonic names. First is the canonical name, e.g. F, not E#
    Strings m_displayNames;

    int m_midiNote = 0; // note value to play when pressed
    float angle = 0; // keys rotate when pressed
    Colour m_colour; // highlighted etc
    PObjMesh m_mesh;

    bool LoadFromString(const std::string& s);
  };

  // 3D keys arranged from lowest to highest
  std::vector<Key> m_keys;
};
}


