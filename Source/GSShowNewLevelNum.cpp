#include <MessageQueue.h>
#include "GSMainCorridor.h"
#include "GSShowNewLevelNum.h"

namespace Amju
{
GSShowNewLevelNum::GSShowNewLevelNum()
{
  m_guiFilename = "Gui/gs_show_new_level_num.txt";
}

void GSShowNewLevelNum::OnActive()
{
  GSBase::OnActive();

  GoTo<TheGSMainCorridor>();

  auto* gs = TheGSMainCorridor::Instance();
  gs->SetPrevState(this);
  // Time delay so we get to see an animation, e.g. door opening
  TheMessageQueue::Instance()->Add(new FuncMsg(GoTo<TheGSMainCorridor>, SecondsFromNow(6)));

}

}

