// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include <GuiText.h>
#include <MessageQueue.h>
#include "GSMainMenu.h"
#include "GSTopicEnd.h"

namespace Amju
{
static void OnOK(GuiElement*)
{
  TheGSTopicEnd::Instance()->ScrollUp();
  TheMessageQueue::Instance()->Add(new FuncMsg(GoTo<TheGSMainMenu>, SecondsFromNow(1.0f)));
}

GSTopicEnd::GSTopicEnd()
{
  m_guiFilename = "Gui/gs_topic_end.txt";
}

void GSTopicEnd::SetScore(int num, int denom)
{
  m_num = num;
  m_denom = denom;
}

void GSTopicEnd::OnActive()
{
  GSBase::OnActive();

  // Set button commands
  GuiElement* elem = GetElementByName(m_gui, "ok-button");
  elem->SetCommand(OnOK);

  // Set score number text
  GuiText* text = (GuiText*)GetElementByName(m_gui, "num-text");
  Assert(text);
  text->SetText(ToString(m_num));

  text = (GuiText*)GetElementByName(m_gui, "denom-text");
  Assert(text);
  text->SetText(ToString(m_denom));

  // Set comment about score etc
  text = (GuiText*)GetElementByName(m_gui, "comment-text");
  Assert(text);
  text->SetText("You suck!");

  // Add to hints (TODO animate additions)

}
}

