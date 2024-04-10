// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

// Implement PlayMidi() using the BASS library. Used for Windows, not iOS.
// The other way to do this, probably better, would be to make a new SoundPlayerImpl. Rats.

#include <iostream>
#include <BassSoundPlayer.h>
#include <File.h>
#include <SoundManager.h>

using namespace Amju;

void PlayMidi(int note, int velocity)
{
  std::cout << "Playing midi note: " << note << " vel: " << velocity << "\n";

  SoundManager* sm = TheSoundManager::Instance();
  if (velocity == 0)
  {
    if (!sm->MidiNoteOff(note)) // Not a great interface
    {
      std::cout << "..midi call failed :(\n";
    }
  }
  else
  {
    if (!sm->MidiNoteOn(note))
    {
      std::cout << "..midi call failed :(\n";
    }
  }
}

