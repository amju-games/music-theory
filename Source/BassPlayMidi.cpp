// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

// Implement PlayMidi() using the BASS library. Used for Windows and Mac, not iOS.
// The other way to do this, probably better, would be to make a new SoundPlayerImpl. Rats.

#ifndef AMJU_IOS

#include <iostream>
#include <File.h>
#include <SoundManager.h>

using namespace Amju;

void PlayMidi(int note, int velocity)
{
#ifdef PLAY_MIDI_DEBUG
  std::cout << "Playing midi note: " << note << " vel: " << velocity << "\n";
#endif

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
    if (!sm->MidiNoteOn(note, velocity))
    {
      std::cout << "..midi call failed :(\n";
    }
  }
}

#endif // AMJU_IOS

