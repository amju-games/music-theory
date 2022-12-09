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
std::string LurkMsg::s_modalBgFilename = "Gui/modal-grey-out.txt";

const char* LurkMsg::MESSAGE_FONT = "font2d/TEXT-font.font";

const float LurkMsg::DEFAULT_MAX_LURK_TIME = 3.0f;

// Extra border around text
static const Vec2f EXTRA(0.1f, 0.05f); // TODO CONFIG

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

LurkMsg::LurkMsg()
{
  m_timer = 0;
  m_state = LURK_NEW;
  m_lurkPos = AMJU_NONE;

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

  Set(text, fgCol, bgCol, lp, maxTime, onOk);
}

bool LurkMsg::IsFinished() const
{
  return m_state == LURK_FINISHED;
}

bool LurkMsg::LoadGui(const std::string& guiFilename)
{
  m_gui = ::Amju::LoadGui(guiFilename, false);
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
  
  return true;
}

void LurkMsg::DrawModalBg()
{
  if (m_isModal)
  {
    PGuiElement bg;
    if (!bg)
    {
      bg = ::Amju::LoadGui(s_modalBgFilename, false);
    }
    bg->Draw();
  }
}

void LurkMsg::Draw()
{
  DrawModalBg();

  m_rect->Draw();
  m_text->Draw();
}

void LurkMsg::Update()
{
  float dt = TheTimer::Instance()->GetDt();

  // Update GUIs
  m_text->Update();
  if (m_gui)
  {
    m_gui->Update();
  }

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
    // For non-centred msgs, there is no button, and we always time out.
    // (TODO for now)
    m_timer += dt;
    if (m_timer > m_maxTime)
    {
      m_state = LURK_HIDING;
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
    else 
    {
      Vec2f  dpos = m_vel * dt;
      m_pos += dpos;
      m_rect->SetLocalPos(m_rect->GetLocalPos() + dpos);
      m_text->SetLocalPos(m_text->GetLocalPos() + dpos);
    }
    break;

  case LURK_HIDING: 
    // Fully hidden ?
    if ((m_lurkPos == AMJU_TOP    && m_pos.y > m_hidePos.y) ||
        (m_lurkPos == AMJU_BOTTOM && m_pos.y < m_hidePos.y) ||
        (m_lurkPos == AMJU_LEFT   && m_pos.x < m_hidePos.x) ||
        (m_lurkPos == AMJU_RIGHT  && m_pos.x > m_hidePos.x))
    {
      m_state = LURK_FINISHED;
    }
    else
    {
      Vec2f  dpos = m_vel * -dt;
      m_pos += dpos;
      m_rect->SetLocalPos(m_rect->GetLocalPos() + dpos);
      m_text->SetLocalPos(m_text->GetLocalPos() + dpos);
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
  const float LURK_MSG_WIDTH = 1.5f;
  
  GuiText* text = new GuiText;

  static const float fontX = ROConfig()->GetFloat("lurk-font-x");
  static const float fontY = ROConfig()->GetFloat("lurk-font-y");

  text->SetFont(nullptr); // cancel default font - obvs should not be required
  text->SetFont(MESSAGE_FONT);
  text->SetFontSize(fontY);
  text->SetScaleX(fontX);

  text->SetSize(Vec2f(LURK_MSG_WIDTH, 0.1f * fontY)); // assume single line
  text->SetText(str);
  text->SizeToText();
  text->SetFgCol(fgCol);

  Set(text, fgCol, bgCol, lp, maxTime, onFinished);
}

void LurkMsg::Set(GuiElement* text, const Colour& fgCol, const Colour& bgCol, LurkPos lp,
  float maxTime, CommandFunc onOk)
{
  const float LURK_MSG_CORNER_RADIUS = 0.04f;
  
  m_text = text;

  m_rect = new GuiRect;
  m_rect->SetSize(m_text->GetSize() + EXTRA);
  m_rect->SetColour(bgCol);
  m_rect->SetCornerRadius(LURK_MSG_CORNER_RADIUS);

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

  case AMJU_NONE:
    Assert(0);
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

Lurker::Lurker()
{
}

void Lurker::OnLurkOk()
{
  Assert(m_qmap.find(AMJU_CENTRE) != m_qmap.end());
  LurkMsgQ& q = m_qmap[AMJU_CENTRE];
  Assert(!q.empty());

  LurkMsg& lm = *(m_qmap[AMJU_CENTRE].front());
  lm.m_state = LurkMsg::LURK_HIDING;
  lm.DoOk();
}

void Lurker::OnLurkYes()
{
  LurkMsg& lm = *(m_qmap[AMJU_CENTRE].front());
  lm.m_state = LurkMsg::LURK_HIDING;
  lm.DoYes();
}

void Lurker::OnLurkNo()
{
  LurkMsg& lm = *(m_qmap[AMJU_CENTRE].front());
  lm.m_state = LurkMsg::LURK_HIDING;
  lm.DoNo();
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
    LurkMsg& msg = *(q.front());
    if (msg.IsFinished())
    {
      q.pop();
    }
    else
    {
      msg.Update();
    }
  }
}

void Lurker::Draw()
{
  for (QMap::iterator it = m_qmap.begin(); it != m_qmap.end(); ++it)
  {
    LurkMsgQ& q = it->second;
    if (q.empty())
    {
      continue;
    }
    LurkMsg& msg = *(q.front());
    msg.Draw();
  }
}

void Lurker::Queue(PLurkMsg lm, bool immediate)
{
  LurkMsgQ& q = m_qmap[lm->m_lurkPos];
  if (immediate)
  {
    while (!q.empty())
    {
      q.pop();
    }
  }

  q.push(lm);
}

void Lurker::Clear()
{
  m_qmap.clear();
}

void SetAsListener(PGuiElement gui)
{
  static EventPoller* ep = TheEventPoller::Instance();

  // Set Lurk Msg as modal, so we can't do anything else until we tap OK
  ep->SetModalListener(gui);
}
} // namespace 

