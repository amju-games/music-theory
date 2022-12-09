// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <map>
#include <queue>
#include <string>
#include <Colour.h>
#include <GuiText.h>
#include <GuiRect.h>
#include <GuiButton.h>
#include <Singleton.h>
#include <StringUtils.h>
#include "LurkMsg.h"

namespace Amju
{
// Message displayed in centre of screen
class CentreMsg : public LurkMsg
{
public:
  CentreMsg();

  // Optional function which is called when message has been displayed and has now gone away
  CentreMsg(const std::string& text, const Colour& fgCol, const Colour& bgCol,  
    float maxTime, CommandFunc onFinished = 0, const std::string& fontName = MESSAGE_FONT);

  void Draw() override;
  void Update() override;

  void SetCentred(const std::string& text, const Colour& fgCol, const Colour& bgCol, 
    float maxTime, CommandFunc onFinished = 0);

  void SetCentred(PGuiElement gui, const Colour& fgCol, const Colour& bgCol, 
    float maxTime, CommandFunc onFinished = 0);

  // Specialised behaviour for centre msg - multi-line text
  virtual void Set(GuiElement* text, const Colour& fgCol, const Colour& bgCol, LurkPos lp,
    float maxTime, CommandFunc onFinished = 0) override;

  // Set to add avatar GUI to message
  void SetAvatarFilename(const std::string& avatarFilename);

  // Convenience function: make a GuiText out of the given string, setting the
  //  fg colour.
  static GuiText* MakeGuiText(const std::string& str, const Colour& col, const std::string& fontName);

  static const float DEFAULT_MAX_LURK_TIME;

private:
  float m_scale = 1.f; // for centre msgs which scale up and down

  // If set, we load this additional GUI to show an avatar
  std::string m_avatarFilename;

  // Tutorial messages are accompanied by an avatar.
  PGuiElement m_avatarGui;
};

void ShowYesNo(const std::string& q, const Colour& fgCol, const Colour& bgCol,
  CommandFunc no, CommandFunc yes);
}

