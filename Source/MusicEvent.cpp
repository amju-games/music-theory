// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <Game.h>
#include "GSBase.h"
#include "MusicEvent.h"

namespace Amju
{
// TODO tie this to difficulty setting, when we add that.
bool IsPlayerPitchCorrect(int playerNote, int scoreNote)
{
  // Crazy idea: ignore the octave, just grade on the step.
  // This solves the problems of fitting all the keys on screen, etc.
  // And a colour palette just needs to cover one octave.
  return (playerNote % 12) == (scoreNote % 12); 
}

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

