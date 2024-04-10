// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include "CentreMsg.h"
#include "Consts.h"
#include "TutorialIds.h"
#include "TutorialManager.h"

namespace Amju
{
/*
    AMJU_FIRST_TIME_THIS_RUN,
    AMJU_FIRST_TIME_THIS_USER,
    AMJU_FIRST_TIME_THIS_INSTALL,
*/

bool TutorialManager::FirstTimeMsg(int msgId, PLurkMsg lm, FirstTimeType ftt)
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
  if (ftt == AMJU_EVERY_TIME)
  {
    // Always return false, meaning, we have not yet seen this message, so we
    //  should show it. This is backwards, this function should be called
    //  ShouldWeShowMsg, and we would then return true here, sigh.
    return false;
  }

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
    const char* TUTORIAL_FONT = "font2d/TEXT-font.font";

    RCPtr<CentreMsg> lm = new CentreMsg(
      LookupTutorialString(id),
      Colour(0, 0, 0, 1), // text colour
      Colour(1, 1, 1, 1), // bg colour
      AMJU_LURK_NO_TIMER, // > 0 for timed msg
      nullptr, // On OK function
      TUTORIAL_FONT); 

    const char* TUTORIAL_GUI = "Gui/tutorial.txt";

    lm->SetAvatarFilename(TUTORIAL_GUI);
    TheTutorialManager::Instance()->FirstTimeMsg(id, lm.GetPtr(), ftt);
  }
}

};

