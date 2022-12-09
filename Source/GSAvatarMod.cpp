// * Amjula music theory *
// (c) Copyright 2017-2018 Jason Colman

#include <GuiButton.h>
#include <GuiDecTranslate.h>
#include <LoadVec2.h>
#include "ColourPicker.h"
#include "GSAvatarMod.h"

namespace Amju
{
GSAvatarMod::GSAvatarMod()
{
  m_guiFilename = "Gui/gs-avatar-mod.txt";
}

void GSAvatarMod::OnActive()
{
  GSBase::OnActive();
  GetElementByName(m_gui, "cancel-button")->SetCommand([this](GuiElement*) { GoBack(); });

  m_avatar = dynamic_cast<GuiAvatar*>(GetElementByName(m_gui, "avatar"));
  Assert(m_avatar);

  SetUpModHead(GetElementByName(m_gui, "avatar-mod-head"));
}

void GSAvatarMod::SetColourPicker(GuiElement* elem, const std::string& avatarColourElementName)
{
  ColourPicker* picker = dynamic_cast<ColourPicker*>(elem);
  Assert(picker);
  picker->SetPickFunc([this, avatarColourElementName](const Colour& col)
  {
    std::string str = "colour;" + avatarColourElementName + ";" + ToHexString(col);
    m_avatar->SetOneDescendant(str);
  });
}

void GSAvatarMod::Translate(const std::string& avatarTranslateElementName, const Vec2f& posChange)
{
  // Name should be of a translate decorator
  GuiDecTranslate* tr = dynamic_cast<GuiDecTranslate*>(GetElementByName(m_avatar, avatarTranslateElementName));
  Assert(tr);
  Vec2f pos = tr->GetTranslation();
  pos += posChange;
  // Set back in avatar, as a string so persistent
  std::string str = "translate;" + avatarTranslateElementName + ";" + ToString(pos);
  m_avatar->SetOneDescendant(str);
}

void GSAvatarMod::SetTranslateButton(
  GuiElement* elem, const std::string& avatarTranslateElementName,
  const std::string& buttonName, const Vec2f& translation)
{
  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(elem, buttonName));
  Assert(button);
  button->SetCommand([=](GuiElement*)
  {
    Translate(avatarTranslateElementName, translation);
  });
}

void GSAvatarMod::SetTranslateButtons(GuiElement* elem, const std::string& avatarTranslateElementName, float distance)
{
  SetTranslateButton(elem, avatarTranslateElementName, "button-dir-left",  Vec2f(-distance, 0));
  SetTranslateButton(elem, avatarTranslateElementName, "button-dir-right", Vec2f( distance, 0));
  SetTranslateButton(elem, avatarTranslateElementName, "button-dir-up",    Vec2f(0,  distance));
  SetTranslateButton(elem, avatarTranslateElementName, "button-dir-down",  Vec2f(0, -distance));
}

void GSAvatarMod::SetUpModHead(GuiElement* gui)
{
  SetTranslateButtons(GetElementByName(gui, "direction-control"), "translate-avatar-head-and-face", 0.01f);
  SetColourPicker(GetElementByName(gui, "colour-picker"), "colour-head");
}


}

