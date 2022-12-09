// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <AmjuFirst.h>
#include <Timer.h>
#include <EventPoller.h>
#include <SoundManager.h>
#include <TextToSpeech.h>
#include "CentreMsg.h"
#include "CentreMsg.h"
#include "PlayWav.h"
#include "ROConfig.h"
#include <AmjuFinal.h>

//#define TEXT_TO_SPEECH

namespace Amju
{
static const char* LURK_GUI_FILENAME = "Gui/gui-lurk.txt";

static const char* LURK_FONT = "font2d/arial-font.font";

const float CentreMsg::DEFAULT_MAX_LURK_TIME = 3.0f;

// Extra border around text
static const Vec2f EXTRA(0.1f, 0.05f); // TODO CONFIG

CentreMsg::CentreMsg(const std::string& text, const Colour& fgCol, const Colour& bgCol, 
  float maxTime, CommandFunc onOk)
{
  m_lurkPos = AMJU_CENTRE;
  SetCentred(text, fgCol, bgCol, maxTime, onOk);
}

void CentreMsg::Draw()
{
  AmjuGL::PushMatrix();
  AmjuGL::Scale(m_scale, m_scale, 1.0f);
  m_rect->Draw();
  if (m_scale > 0.99f)
  {
    m_gui->Draw();
  }
  AmjuGL::PopMatrix();
}

void CentreMsg::Update()
{
  float dt = TheTimer::Instance()->GetDt();

  switch (m_state)
  {
  case LURK_NEW:
    {
      static std::string soundfx = ROConfig()->GetValue("sound-new-lurkmsg");
      PlayWav(soundfx);

      m_state = LURK_SHOWING;
      m_rect->SetVisible(true);
      m_gui->SetVisible(true);

      if (m_gui)
      {
        SetAsListener(m_gui);
      }
    }
    break;

  case LURK_SHOWN:
    // For centred msgs, wait for button click, unless we have set a time out.
    if (m_maxTime > 0)
    {
      m_timer += dt;
      if (m_timer > m_maxTime)
      {
        m_state = LURK_HIDING;
      }
    }
    break;

  case LURK_SHOWING:
    // Fully showing ?
    if (m_scale > 1.0f)
    {
      m_scale = 1.0f;
      m_state = LURK_SHOWN;

#ifdef TEXT_TO_SPEECH
      m_gui->TextToSpeech();
#endif
    }
    else 
    {
      Vec2f  dpos = m_vel * dt;
      m_pos += dpos;
      m_rect->SetLocalPos(m_rect->GetLocalPos() + dpos);
      m_gui->SetLocalPos(m_gui->GetLocalPos() + dpos);

      m_scale += 2.0f * dt; // TODO TEMP TEST
    }
    break;

  case LURK_HIDING: 
    // Fully hidden ?
    if (m_scale < 0.25f)
    {
      m_state = LURK_FINISHED;
      m_scale = 0;

      // Release modal listener status
      if (m_gui) 
      {
        SetAsListener(nullptr);
      }
    }
    else
    {
      Vec2f  dpos = m_vel * -dt;
      m_pos += dpos;
      m_rect->SetLocalPos(m_rect->GetLocalPos() + dpos);
      m_gui->SetLocalPos(m_gui->GetLocalPos() + dpos);

      m_scale -= 2.0f * dt; // TODO TEMP TEST
    }
    break;

  case LURK_FINISHED:
    m_rect->SetVisible(false);
    m_gui->SetVisible(false);
    break;

  case LURK_HIDDEN:
    // Do nothing
    break;

  }
}

void CentreMsg::SetCentred(const std::string& str, const Colour& fgCol, const Colour& bgCol, 
  float maxTime, CommandFunc onFinished)
{
  const float LURK_MSG_WIDTH = 1.5f;
  
  GuiText* text = new GuiText;
  text->SetIsMulti(true);

  static const float fontX = ROConfig()->GetFloat("lurk-font-x");
  static const float fontY = ROConfig()->GetFloat("lurk-font-y");

  text->SetFont(nullptr); // cancel default font - obvs should not be required
  text->SetFont(LURK_FONT);
  text->SetFontSize(fontY);
  text->SetScaleX(fontX);

  text->SetSize(Vec2f(LURK_MSG_WIDTH, 0.1f * fontY)); // assume single line
  text->SetText(str);
  text->SizeToText();
  text->SetFgCol(fgCol);

  SetCentred(text, fgCol, bgCol, maxTime, onFinished);
}

void CentreMsg::SetCentred(PGuiElement gui, const Colour& fgCol, const Colour& bgCol, 
  float maxTime, CommandFunc onOk)
{
  const float LURK_MSG_CORNER_RADIUS = 0.04f;
  
  m_gui = gui;

  m_rect = new GuiRect;
  m_rect->SetSize(m_gui->GetSize() + EXTRA);
  m_rect->SetColour(bgCol);
  m_rect->SetCornerRadius(LURK_MSG_CORNER_RADIUS);

  m_timer = 0;
  m_maxTime = maxTime;
  m_state = LURK_NEW;
  m_onOk = onOk;

  float h = m_gui->GetSize().y;
  float w = m_gui->GetSize().x;

  float yOffset = 0;

  m_showPos = Vec2f(-w * 0.5f, h * 0.5f + yOffset); 
  m_hidePos = m_showPos;
  m_rect->SetRoundCorners(0); 
  m_scale = 0.5f;

  m_vel = (m_showPos - m_hidePos);
  if (m_vel.SqLen() > 0)
  {
    m_vel.Normalise();
    m_vel *= 0.5f; // TODO CONFIG
  }
  m_pos = m_hidePos;
  m_gui->SetLocalPos(m_hidePos); 
  m_rect->SetLocalPos(m_hidePos - 0.5f * Vec2f(EXTRA.x, -EXTRA.y)); 
}

}
