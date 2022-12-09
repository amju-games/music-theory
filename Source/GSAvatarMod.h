// * Amjula music theory *
// (c) Copyright 2017-2018 Jason Colman

#pragma once

#include "GSBase.h"
#include "GuiAvatar.h"

namespace Amju
{
class GSAvatarMod : public GSBase
{
public:
  GSAvatarMod();
  virtual void OnActive() override;

private:
  // Set up buttons for each page
  void SetUpModHead(GuiElement* gui);

  // Util functions
  void SetColourPicker(GuiElement* elem, const std::string& avatarColourElementName);

  void SetTranslateButtons(GuiElement* elem, const std::string& avatarTranslateElementName, float d);
  void SetTranslateButton(
    GuiElement* elem, const std::string& avatarTranslateElementName,
    const std::string& buttonName, const Vec2f& translation);
  void Translate(const std::string& avatarTranslateElementName, const Vec2f& tr);

private:
  RCPtr<GuiAvatar> m_avatar;
};

using TheGSAvatarMod = Singleton<GSAvatarMod>;
}

