#pragma once

#include <GuiDecorator.h>
#include "CurvedPathMixin.h"

namespace Amju
{
// * GuiDecCurvedPath *
// Animates a translation from A to B through a curved path.
class GuiDecCurvedPath : public GuiDecorator, public CurvedPathMixin
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  GuiDecCurvedPath* Clone() override { return new GuiDecCurvedPath(*this); }

  bool Load(File*) override;
  bool Save(File*) override;

  void Animate(float animValue) override;
};
}

