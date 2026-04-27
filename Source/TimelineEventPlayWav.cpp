#include <File.h>
#include "BassPlayMidi.h"
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

const char* TimelineEventPlayMidiSong::NAME = "play-midi";

bool TimelineEventPlayMidiSong::Load(File* f) 
{
  if (!f->GetDataLine(&m_midiSongName))
  {
    f->ReportError("Expected midi song name");
    return false;
  }
  return true;
}

void TimelineEventPlayMidiSong::Execute() 
{
  PlayMidiSong(m_midiSongName);
}

const char* TimelineEventStopMidiSong::NAME = "stop-midi";

void TimelineEventStopMidiSong::Execute() 
{
  StopMidiSong();
}
}

