// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "GuiSetFromString.h"

namespace Amju
{
// * GuiAvatar *
// Avatar with blinking eyes which look around.
// We can set attribs on descendant elements from strings, to give
//  a large combination of different-looking characters.
class GuiAvatar : public GuiSetFromString
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  void Update() override;

  // Eye look direction: (0, 0) is dead ahead; (1, 0) is looking right, etc.
  void SetLookDir(const Vec2f& lookDir);

protected:
 // Call base class version, but with checks for specially named elements
 //  to set symmetrical elements like eyes. 
 bool SetAttribOfNamedElementWithFunc(
   const std::string& name, SetFromStringFunc func, const std::string& value)
   override;
  
private:
  // Eye info - for this to work we rely on specially named elements.
  Vec2f m_lookDir;
  Vec2f m_desiredLookDir;
  float m_blinkTime = 0;
  float m_blinkCheckTime = 0;
};
}
