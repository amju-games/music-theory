// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <Game.h>
#include "GSBase.h"
#include "MusicEvent.h"

namespace Amju
{
void OnMusicKbEvent(const MusicKbEvent& event)
{
  GameState* gs = TheGame::Instance()->GetState();
  GSBase* gsb = dynamic_cast<GSBase*>(gs);
  if (gsb)
  {
    gsb->OnMusicKbEvent(event);
  }
}
}

