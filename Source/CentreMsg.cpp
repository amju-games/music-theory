// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuFirst.h>
#include <Batched.h>
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

namespace
{
const char* MODAL_BUTTON_GUI_FILENAME = "Gui/modal-buttons.txt";

// Extra border around text
const Amju::Vec2f EXTRA(0.1f, 0.05f); // TODO CONFIG

// Lowest position of GUI buttons, so they don't go off screen.
const float MIN_BUTTON_Y = -0.6f;
} // anon namespace

namespace Amju
{
CentreMsg::CentreMsg()
{
  m_lurkPos = AMJU_CENTRE;
  m_isModal = true;
}

CentreMsg::CentreMsg(const std::string& text, const Colour& fgCol, const Colour& bgCol, 
  float maxTime, CommandFunc onOk, const std::string& fontName) : CentreMsg()
{
  m_fontName = fontName;
  SetCentred(text, fgCol, bgCol, maxTime, onOk);
}

void CentreMsg::SetAvatarFilename(const std::string& avatarFilename)
{
  m_avatarFilename = avatarFilename;

  // Load avatar GUI
  if (!m_avatarFilename.empty())
  {
    m_avatarGui = ::Amju::LoadGui(m_avatarFilename, false /* don't listen for events */);
    Assert(m_avatarGui);
  }
}

void CentreMsg::Draw()
{
  DrawModalBg();

  if (m_avatarGui)
  {
    m_avatarGui->Draw();
  }

  // Flush, to make sure that rect/text will appear over the top of 
  //  everything else.
  Batched::DrawAll();

  AmjuGL::PushMatrix();
  AmjuGL::Scale(m_scale, m_scale, 1.0f);
  m_rect->Draw();
  if (m_scale > 0.99f)
  {
    m_text->Draw();
  }
  AmjuGL::PopMatrix();

  // Draw GUI buttons on top
  m_gui->SetVisible(false); // so inert if not displayed

  if (m_state == LurkMsg::LURK_SHOWN)
  {
    m_text->SetVisible(true);
    m_ok->SetIsFocusButton(true);
    m_ok->SetShowIfFocus(true);
    
    // Hide buttons with no command
    
    // Hide OK button if timed
    if (m_maxTime == 0)
    {
      m_gui->SetVisible(true);
      m_gui->Draw();
    }
  }
}

void CentreMsg::Update()
{
  // TODO Just do once when set
  m_ok->SetVisible(m_onOk != nullptr || (m_onYes == nullptr && m_onNo == nullptr));
  m_yes->SetVisible(m_onYes != nullptr);
  m_no->SetVisible(m_onNo != nullptr);

  float dt = TheTimer::Instance()->GetDt();

  // Update GUIs
  m_text->Update();
  if (m_gui)
  {
    m_gui->Update();
  }

  if (m_avatarGui)
  {
    m_avatarGui->Update();
  }

  switch (m_state)
  {
  case LURK_NEW:
    {
      static std::string soundfx = ROConfig()->GetValue("sound-new-lurkmsg");
      PlayWav(soundfx);

      m_state = LURK_SHOWING;
      m_rect->SetVisible(true);
      m_text->SetVisible(true); //? Need this??

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

      // Position GUI under text
      Vec2f pos = m_rect->GetLocalPos(); 
      pos += Vec2f(0, -m_rect->GetSize().y);
      pos.x = 0;
      pos.y = std::max(pos.y, MIN_BUTTON_Y);
      m_gui->SetLocalPos(pos);

#ifdef TEXT_TO_SPEECH
      m_text->TextToSpeech();
#endif
    }
    else 
    {
      Vec2f  dpos = m_vel * dt;
      m_pos += dpos;
      m_rect->SetLocalPos(m_rect->GetLocalPos() + dpos);
//      m_text->SetLocalPos(m_text->GetLocalPos() + dpos);

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
//      m_text->SetLocalPos(m_text->GetLocalPos() + dpos);

      m_scale -= 2.0f * dt; // TODO TEMP TEST
    }
    break;

  case LURK_FINISHED:
    m_rect->SetVisible(false);
    m_text->SetVisible(false);
    break;

  case LURK_HIDDEN:
    // Do nothing
    break;

  }
}

void CentreMsg::SetCentred(
  const std::string& str, const Colour& fgCol, const Colour& bgCol, 
  float maxTime, CommandFunc onFinished)
{
  GuiText* text = MakeGuiText(str, fgCol, m_fontName); 
  SetCentred(text, fgCol, bgCol, maxTime, onFinished);
}

GuiText* CentreMsg::MakeGuiText(const std::string& str, const Colour& fgCol, const std::string& fontName)
{
  const float LURK_MSG_WIDTH = 1.5f;
  
  GuiText* text = new GuiText;
  text->SetIsMulti(true);

  static const float fontX = ROConfig()->GetFloat("lurk-font-x");
  static const float fontY = ROConfig()->GetFloat("lurk-font-y");

  text->SetFont(nullptr); // cancel default font - obvs should not be required
  text->SetFont(fontName);
  text->SetFontSize(fontY);
  text->SetScaleX(fontX);

  text->SetFgCol(fgCol);

  // Set a rough max size, then get the exact size once we have set the text.
  text->SetSize(Vec2f(LURK_MSG_WIDTH, 0.1f * fontY)); // assume single line
  text->SetText(str);
  text->SizeToText();

  return text;
}

void CentreMsg::Set(GuiElement* text, const Colour& fgCol, const Colour& bgCol, LurkPos lp,
  float maxTime, CommandFunc onOk)
{
  SetCentred(text, fgCol, bgCol, maxTime, onOk);
}

void CentreMsg::SetCentred(PGuiElement text, const Colour& fgCol, const Colour& bgCol,
  float maxTime, CommandFunc onOk)
{
  const float LURK_MSG_CORNER_RADIUS = 0.04f;
  
  m_text = text;

  m_rect = new GuiRect;
  Vec2f size = text->CalcRect().GetSize();
  m_rect->SetSize(size + EXTRA);
  m_rect->SetColour(bgCol);
  m_rect->SetCornerRadius(LURK_MSG_CORNER_RADIUS);

  m_timer = 0;
  m_maxTime = maxTime;
  m_state = LURK_NEW;
  m_onOk = onOk;

  float h = size.y;
  float w = size.x;

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
  m_text->SetLocalPos(m_hidePos);
  m_rect->SetLocalPos(m_hidePos - 0.5f * Vec2f(EXTRA.x, -EXTRA.y));
  
  LoadGui(MODAL_BUTTON_GUI_FILENAME);
}

void ShowYesNo(const std::string& q, const Colour& fgCol, const Colour& bgCol,
  CommandFunc no, CommandFunc yes)
{
  // TODO Yes/No msg should be distinct subclass, right?
  PLurkMsg msg = new CentreMsg(q, fgCol, bgCol, 0);
  msg->SetNoCommand(no);
  msg->SetYesCommand(yes);

  TheLurker::Instance()->Queue(msg);
}
}
