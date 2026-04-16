// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

// Implement PlayMidi() using the BASS library.

#include <iostream>

#if defined(MACOSX)|| defined(AMJU_IOS)
// TODO Just for dev - then move this to BassSoundPlayer.
#include "../../../amjulib/Source/SoundBass/Bass2.4/Macosx/bass.h"
#include "../../../amjulib/Source/SoundBass/Bass2.4/Macosx/bassmidi.h"
#endif
#ifdef WIN32
#include "../../../amjulib/Source/SoundBass/Bass2.4/Win/bass.h"
#include "../../../amjulib/Source/SoundBass/Bass2.4/Win/bassmidi.h"
#endif

#include <Directory.h>
#include <File.h>
#include <SoundManager.h>

void PlayMidi(int note, int velocity)
{
  using namespace Amju; // jfc

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

namespace Amju
{
HSOUNDFONT LoadSoundFont(const std::string fontFileName)
{
  auto font = BASS_MIDI_FontInit(fontFileName.c_str(), 0);
  if (font) 
  {
    std::cout << "Loaded soundfont: " << fontFileName << "\n";
  }
  else
  {
    std::cout << "Failed to load soundfont: " << fontFileName << ": ";
    std::cout << "Bass error code: " << BASS_ErrorGetCode() << "\n";
    Assert(0);
  }
  return font;
}

// Apply sound font/channel mapping to the given stream
void MapSoundFontsToChannels(HSTREAM stream)
{
  // Convention for this game:
  // Channel 1 is the player track – muted for game play, but enabled for preview.
  // Channel 2: backing piano LEFT
  // Channel 3: backing piano RIGHT
  // Channel 4: backing piano CENTRE (set up the panning in code/config)
  // Channel 8: backing bass 
  // Channel 10: backing percussion
  auto pianoFont = LoadSoundFont(File::GetRoot() + "Sound/Grand Piano.sf2");
  auto bassFont = LoadSoundFont(File::GetRoot() + "Sound/Colin_s_Double_Bass.sf2");
  auto drumFont = LoadSoundFont(File::GetRoot() + "Sound/Jazz Kit.sf2");

  BASS_MIDI_FONT fonts[3]; // piano, bass, percussion 

  // 1. Load the Piano into the Default Bank (Bank 0)
  // Channels 1-4 will automatically use this because MIDI defaults to Bank 0.
  fonts[0].font = pianoFont;
  fonts[0].preset = -1; 
  fonts[0].bank = 0;    

  // 2. Load the Bass into a Custom Bank (Bank 1)
  fonts[1].font = bassFont;
  fonts[1].preset = -1; 
  fonts[1].bank = 1; 
  
  // 3. Drums (Bank 128) 
  // Even if your font only has one drum kit, Bank 128 is the MIDI standard for drums.
  fonts[2].font = drumFont;
  fonts[2].preset = -1; 
  fonts[2].bank = 128;
  
  // Apply the fonts to the stream
  BASS_MIDI_StreamSetFonts(stream, fonts, 3);
  
  // --- THE ROUTING ---
  // Piano (Ch 1-4) is already on Bank 0.
  
  // Tell Channel 8 (Index 7) to look in Bank 1 instead of Bank 0!
  BASS_MIDI_StreamEvent(stream, 7, MIDI_EVENT_BANK, 1);
  
  // Optional but highly recommended: Force Channel 8 to play the Acoustic Bass preset (32)
  // Just in case the MIDI file sends a program change that messes it up
  BASS_MIDI_StreamEvent(stream, 7, MIDI_EVENT_PROGRAM, 32);
  
  // Route Drums (Ch 10 / Index 9) to Bank 128
  BASS_MIDI_StreamEvent(stream, 9, MIDI_EVENT_BANK, 128);
  
  // ** Panning **
  // Channel 2 (Index 1) -> Hard Left
  BASS_MIDI_StreamEvent(stream, 1, MIDI_EVENT_PAN, 0); 
  // Channel 3 (Index 2) -> Hard Right
  BASS_MIDI_StreamEvent(stream, 2, MIDI_EVENT_PAN, 127);
  // Channel 8 (Index 7) -> Center
  BASS_MIDI_StreamEvent(stream, 7, MIDI_EVENT_PAN, 64);
  
  // Force the "Jazz Kit" (32) or "Brush Kit" (40) on Channel 10
  BASS_MIDI_StreamEvent(stream, 9, MIDI_EVENT_PROGRAM, 32);
  
  // Reverb
  // 0 = No Reverb, 127 = Drowning in a cathedral
  // 40-50 is usually a nice "Small Jazz Club" vibe
  int reverbLevel = 110; //45; 
  
  BASS_MIDI_StreamEvent(stream, 1, MIDI_EVENT_REVERB, reverbLevel); // Piano L
  BASS_MIDI_StreamEvent(stream, 2, MIDI_EVENT_REVERB, reverbLevel); // Piano R
  BASS_MIDI_StreamEvent(stream, 3, MIDI_EVENT_REVERB, reverbLevel); // Piano C
  // Less reverb on low freq instruments..?
  BASS_MIDI_StreamEvent(stream, 7, MIDI_EVENT_REVERB, reverbLevel / 4); // Bass
  BASS_MIDI_StreamEvent(stream, 9, MIDI_EVENT_REVERB, reverbLevel / 4); // Drums
  
  // Add the Reverb effect to the stream
  HFX reverbFX = BASS_ChannelSetFX(stream, BASS_FX_DX8_REVERB, 0);
  
  BASS_DX8_REVERB params;
  params.fInGain = 0.0f;        // Input level
  params.fReverbMix = -20.0f;   // The "Smidgen" (in dB). Try -30 for less, -10 for more.
  params.fReverbTime = 1000.0f; // 1 second decay (good for jazz)
  params.fHighFreqRTRatio = 0.1f;
  
  BASS_FXSetParameters(reverbFX, &params);
}
  
void PlayMidiTrack(const std::string& filename)
{
  // TODO Load from glue file
  HSTREAM mainStream = BASS_MIDI_StreamCreateFile(
    FALSE, 
    filename.c_str(),
    0, 0, BASS_SAMPLE_FLOAT, 1);

  if (mainStream)
  {
    std::cout << "Loaded midi file ok?!? " << filename << "\n";
  }
  else
  {
    std::cout << "Failed to load midi file: " << filename << "\n";
    Assert(0);
  }

  MapSoundFontsToChannels(mainStream);

  BASS_ChannelPlay(mainStream, FALSE /* don't restart the channel */);
}
}

