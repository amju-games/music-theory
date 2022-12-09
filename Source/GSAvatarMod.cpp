// * Amjula music theory *
// (c) Copyright 2017-2018 Jason Colman

#include <GuiButton.h>
#include <GuiDecScale.h>
#include <GuiDecTranslate.h>
#include <GuiSprite.h>
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

  // Set up tabs
  const int NUM_TABS = 12;
  for (int i = 0; i < NUM_TABS; i++)
  {
    auto tabButton = GetElementByName(m_gui, "tab-" + ToString(i + 1));
    Assert(tabButton);
    tabButton->SetCommand([=](GuiElement*) { SetControlPage(float(i)); });
  }

  // Set controls on each page
  SetUpModHead(GetElementByName(m_gui, "avatar-mod-head"));
  SetUpModBody(GetElementByName(m_gui, "avatar-mod-body"));
  SetUpModHair(GetElementByName(m_gui, "avatar-mod-hair"));
  SetUpModEyebrow(GetElementByName(m_gui, "avatar-mod-eyebrow"));
  SetUpModEye(GetElementByName(m_gui, "avatar-mod-eye"));
  SetUpModUpperLid(GetElementByName(m_gui, "avatar-mod-upper-lid"));
  SetUpModLowerLid(GetElementByName(m_gui, "avatar-mod-lower-lid"));
  SetUpModNose(GetElementByName(m_gui, "avatar-mod-nose"));
  SetUpModMouth(GetElementByName(m_gui, "avatar-mod-mouth"));
  SetUpModGlasses(GetElementByName(m_gui, "avatar-mod-glasses"));
}

void GSAvatarMod::SetControlPage(float page)
{
  GuiDecTranslate* tr = dynamic_cast<GuiDecTranslate*>(GetElementByName(m_gui, "translate-control-pages-root"));
  Assert(tr);
  tr->SetTranslation(Vec2f(0, page * 2.f));
}

void GSAvatarMod::SetString()
{
  GuiText* text = dynamic_cast<GuiText*>(GetElementByName(m_gui, "avatar-string"));
  Assert(text);
  text->SetText("Hi!");
}

void GSAvatarMod::SetColourPicker(GuiElement* elem, const std::string& avatarColourElementName)
{
  ColourPicker* picker = dynamic_cast<ColourPicker*>(elem);
  Assert(picker);
  picker->SetPickFunc([this, avatarColourElementName](const Colour& col)
  {
    std::string str = "colour;" + avatarColourElementName + ";" + ToHexString(col);
    m_avatar->SetOneDescendant(str);
    SetString();
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
  SetString();
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

void GSAvatarMod::Scale(const std::string& avatarScaleElementName, const Vec2f& scaleChange)
{
  // Name should be of a scale decorator
  GuiDecScale* sc = dynamic_cast<GuiDecScale*>(GetElementByName(m_avatar, avatarScaleElementName));
  Assert(sc);
  Vec2f scale = sc->GetScale();
  scale += scaleChange;
  // Set back in avatar, as a string so persistent
  std::string str = "scale;" + avatarScaleElementName + ";" + ToString(scale);
  m_avatar->SetOneDescendant(str);
  SetString();
}

void GSAvatarMod::SetScaleButton(
  GuiElement* elem, const std::string& avatarScaleElementName,
  const std::string& buttonName, const Vec2f& scale)
{
  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(elem, buttonName));
  Assert(button);
  button->SetCommand([=](GuiElement*)
  {
    Scale(avatarScaleElementName, scale);
  });
}

void GSAvatarMod::SetScaleButtons(GuiElement* elem, const std::string& avatarScaleElementName, float change)
{
  SetScaleButton(elem, avatarScaleElementName, "button-scale-left" , Vec2f(-change, 0));
  SetScaleButton(elem, avatarScaleElementName, "button-scale-right", Vec2f( change, 0));
  SetScaleButton(elem, avatarScaleElementName, "button-scale-up",    Vec2f(0,  change));
  SetScaleButton(elem, avatarScaleElementName, "button-scale-down",  Vec2f(0, -change));
}

void GSAvatarMod::ChangeCell(const std::string& avatarSpriteName, int cellChange, int range)
{
  // Name should be of a sprite 
  GuiSprite* sprite = dynamic_cast<GuiSprite*>(GetElementByName(m_avatar, avatarSpriteName));
  Assert(sprite);
  int cell = sprite->GetCell();
  cell += cellChange;
  // Set back in avatar, as a string so persistent
  std::string str = "cell;" + avatarSpriteName + ";" + ToString(cell) + ", " + ToString(cell + range - 1);
  m_avatar->SetOneDescendant(str);
  SetString();
}

void GSAvatarMod::SetCellButton(
  GuiElement* elem, 
  const std::string& avatarSpriteName,
  const std::string& buttonName, 
  int cellChange,
  int range)
{
  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(elem, buttonName));
  Assert(button);
  button->SetCommand([=](GuiElement*)
  {
    ChangeCell(avatarSpriteName, cellChange, range);
  });
}

void GSAvatarMod::SetCellButtons(GuiElement* elem, const std::string& avatarSpriteName, int range)
{
  // If range is 1, the sprite does not animate. If range is 2, the sprite animates between 2 cells, etc.
  // If range is > 1, we increment/decrement by range, not 1. So, e.g. upper eyelid animations come in 
  //  pairs of cells.
  SetCellButton(elem, avatarSpriteName, "button-cell-left",  -range, range);
  SetCellButton(elem, avatarSpriteName, "button-cell-right",  range, range);
}

void GSAvatarMod::SetUpModHead(GuiElement* gui)
{
  SetTranslateButtons(GetElementByName(gui, "direction-control"), "translate-avatar-head-and-face", 0.01f);
  SetScaleButtons(GetElementByName(gui, "scale-control"), "scale-head", 0.01f);
  SetColourPicker(GetElementByName(gui, "colour-picker"), "colour-scale-head");
}

void GSAvatarMod::SetUpModBody(GuiElement* gui)
{
  SetTranslateButtons(GetElementByName(gui, "direction-control"), "translate-colour-scale-body", 0.01f);
  SetScaleButtons(GetElementByName(gui, "scale-control"), "scale-body", 0.01f);
  SetColourPicker(GetElementByName(gui, "colour-picker"), "colour-scale-body");
}

void GSAvatarMod::SetUpModHair(GuiElement* gui)
{
  SetTranslateButtons(GetElementByName(gui, "direction-control"), "translate-colour-scale-hair", 0.01f);
  SetScaleButtons(GetElementByName(gui, "scale-control"), "scale-hair", 0.01f);
  SetColourPicker(GetElementByName(gui, "colour-picker"), "colour-scale-hair");
  SetCellButtons(GetElementByName(gui, "cell-control"), "hair", 1);
}

void GSAvatarMod::SetUpModEyebrow(GuiElement* gui)
{
  SetTranslateButtons(GetElementByName(gui, "direction-control"), "translate-scale-eyebrow", 0.01f);
  SetScaleButtons(GetElementByName(gui, "scale-control"), "scale-eyebrow", 0.01f);
  SetColourPicker(GetElementByName(gui, "colour-picker"), "colour-translate-scale-eyebrow");
  SetCellButtons(GetElementByName(gui, "cell-control"), "eyebrow", 1);
}

void GSAvatarMod::SetUpModEye(GuiElement* gui)
{
  SetTranslateButtons(GetElementByName(gui, "direction-control"), "translate-avatar-eye-simple", 0.01f);
  SetScaleButtons(GetElementByName(gui, "scale-control"), "scale-translate-avatar-eye-simple", 0.01f); // TODO TEMP TEST
  SetColourPicker(GetElementByName(gui, "colour-picker"), "colour-scale-iris");
}

void GSAvatarMod::SetUpModUpperLid(GuiElement* gui)
{
  SetColourPicker(GetElementByName(gui, "colour-picker"), "colour-scale-eyelid-upper");
  SetCellButtons(GetElementByName(gui, "cell-control"), "eyelid-upper", 2);
}

void GSAvatarMod::SetUpModLowerLid(GuiElement* gui)
{
  SetColourPicker(GetElementByName(gui, "colour-picker"), "colour-scale-eyelid-lower");
}

void GSAvatarMod::SetUpModNose(GuiElement* gui)
{
  SetTranslateButtons(GetElementByName(gui, "direction-control"), "translate-colour-scale-nose", 0.01f);
  SetScaleButtons(GetElementByName(gui, "scale-control"), "scale-nose", 0.01f);
  SetColourPicker(GetElementByName(gui, "colour-picker"), "colour-scale-nose");
  SetCellButtons(GetElementByName(gui, "cell-control"), "nose", 1);
}

void GSAvatarMod::SetUpModMouth(GuiElement* gui)
{
  SetTranslateButtons(GetElementByName(gui, "direction-control"), "translate-colour-scale-mouth", 0.01f);
  SetScaleButtons(GetElementByName(gui, "scale-control"), "scale-mouth", 0.01f);
  SetColourPicker(GetElementByName(gui, "colour-picker"), "colour-scale-mouth");
  SetCellButtons(GetElementByName(gui, "cell-control"), "mouth", 1);
}

void GSAvatarMod::SetUpModGlasses(GuiElement* gui)
{
  SetTranslateButtons(GetElementByName(gui, "direction-control"), "translate-colour-scale-mouth", 0.01f);
  SetScaleButtons(GetElementByName(gui, "scale-control"), "scale-eyebrow", 0.01f);
  SetColourPicker(GetElementByName(gui, "colour-picker"), "colour-scale-mouth");
  SetCellButtons(GetElementByName(gui, "cell-control"), "mouth", 1);
}

}

