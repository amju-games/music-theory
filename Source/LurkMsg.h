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

namespace Amju
{
enum LurkPos { AMJU_NONE, AMJU_TOP, AMJU_BOTTOM, AMJU_LEFT, AMJU_RIGHT, AMJU_CENTRE };

static const float AMJU_LURK_NO_TIMER = 0.f;

// Lurk on edge of screen
class LurkMsg : public RefCounted
{
public:
  LurkMsg();
  // Optional function which is called when message has been displayed and has now gone away
  LurkMsg(const std::string& text, const Colour& fgCol, const Colour& bgCol, LurkPos lp, 
    float maxTime, CommandFunc onFinished = 0);

  // Load GUI from file - currently only for CentreMsgs, but in theory other Msgs could
  //  also load a GUI.
  bool LoadGui(const std::string& guiFilename);
  
  virtual void Draw();
  virtual void Update();
  virtual void Set(const std::string& text, const Colour& fgCol, const Colour& bgCol, LurkPos lp,
    float maxTime, CommandFunc onFinished = 0);

  void Set(GuiText* text, const Colour& fgCol, const Colour& bgCol, LurkPos lp,
    float maxTime, CommandFunc onFinished = 0);

  bool IsFinished() const;

  void SetOkCommand(CommandFunc onOk);
  void SetYesCommand(CommandFunc onYes);
  void SetNoCommand(CommandFunc onNo);
 
  void DoOk();
  void DoYes();
  void DoNo();

  static const float DEFAULT_MAX_LURK_TIME;

protected:
  PGuiElement m_text;
  RCPtr<GuiRect> m_rect;

  Vec2f m_pos; // current pos
  Vec2f m_showPos; // position when LURK_SHOWN state is reached
  Vec2f m_hidePos; // position when fully hidden
  Vec2f m_vel;

  float m_timer = 0;
  float m_maxTime = 0;
  
  enum State { LURK_NEW, LURK_SHOWN, LURK_SHOWING, LURK_HIDDEN, LURK_HIDING, LURK_FINISHED };
  State m_state;

  friend class Lurker;
  LurkPos m_lurkPos;

  // next/ok button - currently for Centred text only
  PGuiElement m_gui;
  // yes/no/ok buttons - TODO something a bit more flexible
  GuiButton* m_ok;
  GuiButton* m_yes;
  GuiButton* m_no;

  // Funcs to call on button press
  CommandFunc m_onOk;
  CommandFunc m_onYes;
  CommandFunc m_onNo;
};

using PLurkMsg = RCPtr<LurkMsg>;

class Lurker : public NonCopyable
{
  Lurker();
  friend class Singleton<Lurker>;

public:
  void Update();
  void Draw();

  // If immediate, preceding messages in the queue are removed
  void Queue(PLurkMsg lm, bool immediate = false);

  void ShowYesNo(const std::string& q, const Colour& fgCol, const Colour& bgCol, 
    CommandFunc no, CommandFunc yes);

  void Clear(); // to prevent old msgs from stacking up

  void OnLurkOk();
  void OnLurkYes();
  void OnLurkNo();

  void TextToSpeech(const std::string& text);

private:
  typedef std::queue<PLurkMsg> LurkMsgQ; // queue of msgs or one Lurk pos
  typedef std::map<LurkPos, LurkMsgQ> QMap; 
  QMap m_qmap; // one queue for each position
};

typedef Singleton<Lurker> TheLurker;

// Convenience function: set the given GUI as modal listener, or 
//  release current modal listener if nullptr.
void SetAsListener(PGuiElement gui);
}

