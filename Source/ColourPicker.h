// * Amjula music theory *
// (c) Copyright 2017-2018 Jason Colman

#pragma once

#include <functional>
#include <Colour.h>
#include <GuiDecInclude.h>

namespace Amju
{
class ColourPicker : public GuiDecInclude
{
public:
  static const char* NAME;

  bool Load(File*) override;

  using OnColourPickFunc = std::function<void(const Colour&)>;
  void SetPickFunc(OnColourPickFunc f) { m_onColourPickFunc = f; }

protected:
  OnColourPickFunc m_onColourPickFunc;
};
}

