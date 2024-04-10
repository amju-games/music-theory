// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include "GSPages.h"
#include "MusicEvent.h"

namespace Amju
{
void OnMusicKbEvent(const MusicKbEvent& event)
{
  TheGSPages::Instance()->OnMusicKbEvent(event);
}
}

