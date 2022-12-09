#include <iostream>
#include "PlayWav.h"
#include <SoundManager.h>

//#define DEBUG_SOUND

namespace Amju
{
void PlayWav(const std::string& wavNoFileExt)
{
#ifdef GEKKO
  // ?
  std::string s = "sound/wii/" + wavNoFileExt + ".snd";
#else
  std::string s = "Sound/wav/" + wavNoFileExt + ".wav";
#endif

#ifdef DEBUG_SOUND
  std::cout << "Playing sound: " << s << "\n";
#endif

  TheSoundManager::Instance()->PlayWav(s);
}
}
