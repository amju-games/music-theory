#include <File.h>
#include "PlayWav.h"
#include "TimelineEventPlayWav.h"

namespace Amju
{
const char* TimelineEventPlayWav::NAME = "play-wav";

bool TimelineEventPlayWav::Load(File* f)
{
  if (!f->GetDataLine(&m_wavName))
  {
    f->ReportError("Expected wav name");
    return false;
  }
  return true;
}

void TimelineEventPlayWav::Execute()
{
  PlayWav(m_wavName);
}
}
