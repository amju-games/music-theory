// * Amjula music theory *
// (c) Copyright 2026 Juliet Colman

#pragma once

#include <GuiComposite.h>
#include "GuiMusicKbBase.h"

namespace Amju
{
// * GuiMusic2dKeyboard *
// 2D piano keyboard
class GuiMusic2dKeyboard : public GuiMusicKbBase
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  virtual void Draw() override;
  virtual bool Load(File*) override;

  // Not polymorphic, just additive
  struct Key2d : public GuiMusicKbBase::Key
  {
    RCPtr<GuiComposite> m_keyComp;

    bool Load(const std::string& s, float xPos, bool isBlack);
  };

private:
  void DrawKeys(bool blackNotWhite);
};
}

