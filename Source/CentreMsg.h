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
    float maxTime, CommandFunc onFinished = 0);

  void Draw() override;
  void Update() override;

  void SetCentred(const std::string& text, const Colour& fgCol, const Colour& bgCol, 
    float maxTime, CommandFunc onFinished = 0);

  void SetCentred(PGuiElement gui, const Colour& fgCol, const Colour& bgCol, 
    float maxTime, CommandFunc onFinished = 0);

  virtual void Set(GuiElement* text, const Colour& fgCol, const Colour& bgCol, LurkPos lp,
    float maxTime, CommandFunc onFinished = 0);

  static const float DEFAULT_MAX_LURK_TIME;

private:
  float m_scale = 1.f; // for centre msgs which scale up and down
};

void ShowYesNo(const std::string& q, const Colour& fgCol, const Colour& bgCol,
  CommandFunc no, CommandFunc yes);
}

