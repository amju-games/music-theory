// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "Consts.h"
#include "LurkMsg.h"
#include "TutorialIds.h"
#include "TutorialManager.h"

namespace Amju
{
/*
    AMJU_FIRST_TIME_THIS_RUN,
    AMJU_FIRST_TIME_THIS_USER,
    AMJU_FIRST_TIME_THIS_INSTALL,
*/

bool TutorialManager::FirstTimeMsg(int msgId, const LurkMsg& lm, FirstTimeType ftt)
{
  if (MsgHasBeenShown(msgId, ftt))
  {
    return false; // already shown, false means we didn't show it again
  }

  TheLurker::Instance()->Queue(lm);

  bool storedOk = SetMsgWasShown(msgId, ftt);
  Assert(storedOk);
  return true;
}

bool TutorialManager::SetMsgWasShown(int msgId, FirstTimeType ftt)
{
  m_shown[ftt].insert(msgId);
  return Save();
}

bool TutorialManager::MsgHasBeenShown(int msgId, FirstTimeType ftt) const
{
  Assert(ftt < AMJU_FIRST_TIME_MAX);
  const auto& set = m_shown[ftt];
  bool shown = set.count(msgId) > 0;
  return shown;
}

bool TutorialManager::Load()
{
  return true;
}

bool TutorialManager::Save()
{
  return true;
}

void QueueFirstTimeMsgs(const std::vector<int> ids, FirstTimeType ftt)
{
  for (int id : ids)
  {
    LurkMsg lm(
      LookupTutorialString(id),
      GetColour(COLOUR_TEXT),
      GetColour(COLOUR_TUTORIAL),
      AMJU_CENTRE,
      2.0f); // TODO TEMP TEST - try timed centred msg

//      AMJU_LURK_NO_TIMER); // TODO allow timed centre msgs

    TheTutorialManager::Instance()->FirstTimeMsg(id, lm, ftt);
  }
}

};

