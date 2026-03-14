// * Amjula music theory *
// (c) Copyright 2026 Juliet Colman

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
//  virtual void Update() override;
  virtual bool Load(File*) override;

  struct Key3d : public GuiMusicKbBase::Key
  {
    PObjMesh m_mesh;

    bool LoadFromString(const std::string& s);
    void CalcRect();

//    void Press() override;
//    void Release() override;
  };

protected:
  // Find key picked by user
  Key* PickKey(const Vec2f& pos) override;
};
}

