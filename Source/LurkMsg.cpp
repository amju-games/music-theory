// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <AmjuFirst.h>
#include <Timer.h>
#include <EventPoller.h>
#include <SoundManager.h>
#include <TextToSpeech.h>
#include "ROConfig.h"
#include "LurkMsg.h"
#include "PlayWav.h"
#include <AmjuFinal.h>

//#define TEXT_TO_SPEECH

namespace Amju
{
static const char* LURK_GUI_FILENAME = "Gui/gui-lurk.txt";

const float LurkMsg::DEFAULT_MAX_LURK_TIME = 3.0f;

// Extra border around text
static const Vec2f EXTRA(0.1f, 0.05f); // TODO CONFIG

LurkMsg::LurkMsg()
{
  m_timer = 0;
  m_state = LURK_NEW;
  m_lurkPos = AMJU_NONE;
  m_scale = 1.0f;

  m_onOk = 0;
  m_onYes = 0;
  m_onNo = 0;
}

LurkMsg::LurkMsg(const std::string& text, const Colour& fgCol, const Colour& bgCol, LurkPos lp,
  float maxTime, CommandFunc onOk)
{
  m_onOk = 0;
  m_onYes = 0;
  m_onNo = 0;

  m_scale = 1.0f;
  Set(text, fgCol, bgCol, lp, maxTime, onOk);
}

bool LurkMsg::IsFinished() const
{
  return m_state == LURK_FINISHED;
}

void LurkMsg::Draw()
{
  AmjuGL::PushMatrix();
  AmjuGL::Scale(m_scale, m_scale, 1.0f);
  m_rect->Draw();
  if (m_scale > 0.99f)
  {
    m_text->Draw();
  }
  AmjuGL::PopMatrix();
}

void LurkMsg::Update()
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
    m_text->SetVisible(true);
    }
    break;

  case LURK_SHOWN:
    if (m_lurkPos != AMJU_CENTRE) // wait for button click. TODO allow timed CENTRE msgs
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
    if ((m_lurkPos == AMJU_TOP    && m_pos.y < m_showPos.y) ||
        (m_lurkPos == AMJU_BOTTOM && m_pos.y > m_showPos.y) ||
        (m_lurkPos == AMJU_LEFT   && m_pos.x > m_showPos.x) ||
        (m_lurkPos == AMJU_RIGHT  && m_pos.x < m_showPos.x))
    {
      m_rect->SetLocalPos(m_showPos - 0.5f * Vec2f(EXTRA.x, -EXTRA.y));
      m_text->SetLocalPos(m_showPos);

#ifdef TEXT_TO_SPEECH
      m_text->TextToSpeech();
#endif

      m_state = LURK_SHOWN;
    }
    else if (m_lurkPos == AMJU_CENTRE && m_scale > 1.0f)
    {
      m_scale = 1.0f;
      m_state = LURK_SHOWN;

#ifdef TEXT_TO_SPEECH
      m_text->TextToSpeech();
#endif
    }
    else 
    {
      Vec2f  dpos = m_vel * dt;
      m_pos += dpos;
      m_rect->SetLocalPos(m_rect->GetLocalPos() + dpos);
      m_text->SetLocalPos(m_text->GetLocalPos() + dpos);

      if (m_lurkPos == AMJU_CENTRE)
      {
        m_scale += 2.0f * dt; // TODO TEMP TEST
      }
    }
    break;

  case LURK_HIDING: 
    // Fully hidden ?
    if ((m_lurkPos == AMJU_TOP    && m_pos.y > m_hidePos.y) ||
        (m_lurkPos == AMJU_BOTTOM && m_pos.y < m_hidePos.y) ||
        (m_lurkPos == AMJU_LEFT   && m_pos.x < m_hidePos.x) ||
        (m_lurkPos == AMJU_RIGHT  && m_pos.x > m_hidePos.x) ||
        (m_lurkPos == AMJU_CENTRE && m_scale < 0.25f))
    {
      m_state = LURK_FINISHED;
      m_scale = 0;
    }
    else
    {
      Vec2f  dpos = m_vel * -dt;
      m_pos += dpos;
      m_rect->SetLocalPos(m_rect->GetLocalPos() + dpos);
      m_text->SetLocalPos(m_text->GetLocalPos() + dpos);

      if (m_lurkPos == AMJU_CENTRE)
      {
        m_scale -= 2.0f * dt; // TODO TEMP TEST
      }
    }
    break;

  case LURK_FINISHED:
    m_rect->SetVisible(false);
    m_text->SetVisible(false);
    break;
  }
}

void LurkMsg::SetOkCommand(CommandFunc onOk)
{
  m_onOk = onOk;
}

void LurkMsg::SetYesCommand(CommandFunc onYes)
{
  m_onYes = onYes;
}

void LurkMsg::SetNoCommand(CommandFunc onNo)
{
  m_onNo = onNo;
}

void LurkMsg::DoOk()
{
  if (m_onOk)
  {
    m_onOk(nullptr);
  }
}

void LurkMsg::DoYes()
{
  if (m_onYes)
  {
    m_onYes(nullptr);
  }
}

void LurkMsg::DoNo()
{
  if (m_onNo)
  {
    m_onNo(nullptr);
  }
}

void LurkMsg::Set(const std::string& str, const Colour& fgCol, const Colour& bgCol, LurkPos lp,
  float maxTime, CommandFunc onFinished)
{
  GuiText* text = new GuiText;
  if (lp == AMJU_CENTRE)
  {
    text->SetIsMulti(true);
  }

  static const float fontX = ROConfig()->GetFloat("lurk-font-x");
  static const float fontY = ROConfig()->GetFloat("lurk-font-y");

  text->SetFontSize(fontY);
  text->SetScaleX(fontX);

  text->SetSize(Vec2f(1.6f, 0.1f * fontY)); // assume single line
  text->SetText(str);
  text->SizeToText();
  text->SetFgCol(fgCol);

  Set(text, fgCol, bgCol, lp, maxTime, onFinished);
}

void LurkMsg::Set(GuiText* text, const Colour& fgCol, const Colour& bgCol, LurkPos lp,
  float maxTime, CommandFunc onOk)
{
  m_text = text;

  m_rect = new GuiRect;
  m_rect->SetSize(m_text->GetSize() + EXTRA);
  m_rect->SetColour(bgCol);
  m_rect->SetCornerRadius(0.04f); // TODO CONFIG

  m_lurkPos = lp;
  m_timer = 0;
  m_maxTime = maxTime;
  m_state = LURK_NEW;
  m_onOk = onOk;

  float h = m_text->GetSize().y;
  float w = m_text->GetSize().x;

  switch (m_lurkPos)
  {
  case AMJU_TOP:
    m_showPos = Vec2f(m_rect->GetSize().x * -0.5f, 1.0f);
    m_hidePos = Vec2f(m_showPos.x, 1.0f + h + EXTRA.y);
    m_rect->SetRoundCorners(GuiRect::AMJU_TL | GuiRect::AMJU_TR);
    break;

  case AMJU_BOTTOM:
    m_showPos = Vec2f(m_rect->GetSize().x * -0.5f, -1.0f + h);
    m_hidePos = Vec2f(m_showPos.x, -1.0f - EXTRA.y);
    m_rect->SetRoundCorners(GuiRect::AMJU_BL | GuiRect::AMJU_BR);
    break;

  case AMJU_LEFT:
    m_showPos = Vec2f(-1.0f, h * 0.5f);
    m_hidePos = Vec2f(-1.0f - w - EXTRA.x, m_showPos.y);
    m_rect->SetRoundCorners(GuiRect::AMJU_BL | GuiRect::AMJU_TL);
    break;

  case AMJU_RIGHT:
    m_showPos = Vec2f(1.0f - w, h * 0.5f);
    m_hidePos = Vec2f(1.0f + EXTRA.x, m_showPos.y);
    m_rect->SetRoundCorners(GuiRect::AMJU_BR | GuiRect::AMJU_TR);
    break;

  case AMJU_CENTRE:
    float yOffset = 0;

    m_showPos = Vec2f(-w * 0.5f, h * 0.5f + yOffset); 
    m_hidePos = m_showPos;
    m_rect->SetRoundCorners(0); 
    m_scale = 0.5f;
    break;
  }

  m_vel = (m_showPos - m_hidePos);
  if (m_vel.SqLen() > 0)
  {
    m_vel.Normalise();
    m_vel *= 0.5f; // TODO CONFIG
  }
  m_pos = m_hidePos;
  m_text->SetLocalPos(m_hidePos); 
  m_rect->SetLocalPos(m_hidePos - 0.5f * Vec2f(EXTRA.x, -EXTRA.y)); 
}

static void OnLurkOk(GuiElement*)
{
  TheLurker::Instance()->OnLurkOk();
}

static void OnLurkYes(GuiElement*)
{
  TheLurker::Instance()->OnLurkYes();
}

static void OnLurkNo(GuiElement*)
{
  TheLurker::Instance()->OnLurkNo();
}

Lurker::Lurker()
{
  m_gui = LoadGui(LURK_GUI_FILENAME, false);
  Assert(m_gui);

  m_ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  Assert(m_ok);
  m_ok->SetCommand(Amju::OnLurkOk);

  m_yes = (GuiButton*)GetElementByName(m_gui, "yes-button");
  Assert(m_yes);
  m_yes->SetCommand(Amju::OnLurkYes);

  m_no = (GuiButton*)GetElementByName(m_gui, "no-button");
  Assert(m_no);
  m_no->SetCommand(Amju::OnLurkNo);

/*
  m_button = new GuiButton;
  m_button->OpenAndLoad("lurk-button.txt");
  TheEventPoller::Instance()->AddListener(m_button);
  m_button->SetVisible(false);
  m_button->SetCommand(Amju::OnLurkOk);
*/
}

void Lurker::OnLurkOk()
{
  Assert(m_qmap.find(AMJU_CENTRE) != m_qmap.end());
  LurkMsgQ& q = m_qmap[AMJU_CENTRE];
  Assert(!q.empty());

  LurkMsg& lm = m_qmap[AMJU_CENTRE].front();
  lm.m_state = LurkMsg::LURK_HIDING;
  lm.DoOk();

  m_gui->SetVisible(true); // ?
}

void Lurker::OnLurkYes()
{
  LurkMsg& lm = m_qmap[AMJU_CENTRE].front();
  lm.m_state = LurkMsg::LURK_HIDING;
  lm.DoYes();

  m_gui->SetVisible(true); // ?
}

void Lurker::OnLurkNo()
{
  LurkMsg& lm = m_qmap[AMJU_CENTRE].front();
  lm.m_state = LurkMsg::LURK_HIDING;
  lm.DoNo();

  m_gui->SetVisible(true); // ?
}

void Lurker::Update()
{
  for (QMap::iterator it = m_qmap.begin(); it != m_qmap.end(); ++it)
  {
    LurkMsgQ& q = it->second;
    if (q.empty())
    {
      continue;
    }
    LurkMsg& msg = q.front();
    if (msg.IsFinished())
    {
      q.pop();
    }
    else
    {
      msg.Update();
      m_ok->SetVisible(msg.m_onOk != 0 || (msg.m_onYes == 0 && msg.m_onNo == 0));
      m_yes->SetVisible(msg.m_onYes != 0);
      m_no->SetVisible(msg.m_onNo != 0);
    }
  }
}

void Lurker::Draw()
{
  m_gui->SetVisible(false); // so inert if not displayed

  for (QMap::iterator it = m_qmap.begin(); it != m_qmap.end(); ++it)
  {
    LurkMsgQ& q = it->second;
    if (q.empty())
    {
      continue;
    }
    LurkMsg& msg = q.front();
    msg.Draw();
    if (msg.m_lurkPos == AMJU_CENTRE && msg.m_state == LurkMsg::LURK_SHOWN)
    {
      Vec2f pos = msg.m_rect->GetLocalPos(); 
      pos += Vec2f(0, -msg.m_rect->GetSize().y);
      pos.x = 0;
      m_gui->SetLocalPos(pos);
      m_gui->SetVisible(true);
      m_ok->SetIsFocusButton(true);
      m_ok->SetShowIfFocus(true);

      // Hide buttons with no command

      m_gui->SetVisible(true);
      m_gui->Draw();
    }
  }
}

void Lurker::Queue(const LurkMsg& lm, bool immediate)
{
  LurkMsgQ& q = m_qmap[lm.m_lurkPos];
  if (immediate)
  {
    while (!q.empty())
    {
      q.pop();
    }
  }

  q.push(lm);
}

void Lurker::ShowYesNo(const std::string& q, const Colour& fgCol, const Colour& bgCol, 
  CommandFunc no, CommandFunc yes)
{
  LurkMsg msg(q, fgCol, bgCol,AMJU_CENTRE, 0);
  msg.SetNoCommand(no);
  msg.SetYesCommand(yes);

  Queue(msg);
}

void Lurker::Clear()
{
  m_qmap.clear();
}

bool Lurker::IsDisplayingMsg() const
{
  QMap::const_iterator it = m_qmap.find(AMJU_CENTRE);
  if (it != m_qmap.end())
  {
    const LurkMsgQ& q = it->second;
    if (!q.empty())
    {
      return true;
    }
  }
  return false;
}

void Lurker::SetAsListener(bool listen)
{
  static EventPoller* ep = TheEventPoller::Instance();

  if (listen)
  {
    if (ep->HasListener(m_gui))
    {
std::cout << "Lurk gui already a listener\n";
    }
    else
    {
      ep->AddListener(m_gui);
    }
  }
  else
  {
    if (ep->HasListener(m_gui))
    {
      ep->RemoveListener(m_gui);
    }
    else
    {
std::cout << "Lurk gui not a listener, can't remove\n";
    }
  }
}

//void Lurker::TextToSpeech(const std::string& text)
//{
//  if (!text.empty())
//  {
//    Amju::TextToSpeech(text);
//  }
//}

}



