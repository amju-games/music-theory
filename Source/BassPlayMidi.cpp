// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

// Implement PlayMidi() using the BASS library. Used for Windows, not iOS.
// The other way to do this, probably better, would be to make a new SoundPlayerImpl. Rats.

#include <BassSoundPlayer.h>
#include <File.h>
#include <SoundManager.h>

using namespace Amju;

void PlayMidi(int note, int velocity)
{
  SoundManager* sm = TheSoundManager::Instance();
  if (velocity == 0)
  {
    sm->MidiNoteOff(note); // Not a great interface
  }
  else
  {
    sm->MidiNoteOn(note);
  }
}

