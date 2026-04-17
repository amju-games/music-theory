// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

// Implement PlayMidi() using the BASS library.

#include <iostream>
#include <unordered_map>

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
#include <GlueFile.h>
#include <SoundManager.h>

namespace Amju
{
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

HSOUNDFONT LoadSoundFont(const std::string fontFileName)
{
#ifdef AMJU_IOS
  std::string prefix = File::GetRoot();
#else
  std::string prefix = File::GetRoot() + "Sound/";
#endif

  auto filename = prefix + fontFileName;

  // TODO Use this map so we only ever load a soundfont once.
  static std::unordered_map<std::string, HSOUNDFONT> fontPool;
  auto it = fontPool.find(filename);
  if (it != fontPool.end())
  {
    std::cout << "Sound font already loaded and in pool: " << fontFileName << "\n";
    return it->second;
  }

  auto font = BASS_MIDI_FontInit(filename.c_str(), 0);
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

  fontPool[filename] = font; // store for next time
  return font;
}

void RouteInstruments(HSTREAM stream)
{
  // Apply our routing so channel 8 is bass, channel 10 is drums.

  // Route Bass (Ch 8 / Index 7) back to Bank 1
  BASS_MIDI_StreamEvent(stream, 7, MIDI_EVENT_BANK, 1);
    
  // Force Channel 8 to play the Acoustic Bass preset (32)
  // Just in case the MIDI file sends a program change that messes it up
  BASS_MIDI_StreamEvent(stream, 7, MIDI_EVENT_PROGRAM, 32);

  // Route Drums (Ch 10 / Index 9) back to Bank 128
  BASS_MIDI_StreamEvent(stream, 9, MIDI_EVENT_BANK, 128);

  // Re-force the Program (Instrument) 
  // This ensures they don't default to Piano (0) if the seek 
  // landed exactly after a Program Change event.
  BASS_MIDI_StreamEvent(stream, 7, MIDI_EVENT_PROGRAM, 32); 
  BASS_MIDI_StreamEvent(stream, 9, MIDI_EVENT_PROGRAM, 0);

  // ** Panning **
  // Channel 2 (Index 1) -> Hard Left
  BASS_MIDI_StreamEvent(stream, 1, MIDI_EVENT_PAN, 0); 
  // Channel 3 (Index 2) -> Hard Right
  BASS_MIDI_StreamEvent(stream, 2, MIDI_EVENT_PAN, 127);
  // Channel 8 (Index 7) -> Center
  BASS_MIDI_StreamEvent(stream, 7, MIDI_EVENT_PAN, 64);
  
  // Force the "Jazz Kit" (32) or "Brush Kit" (40) on Channel 10
//  BASS_MIDI_StreamEvent(stream, 9, MIDI_EVENT_PROGRAM, 32);
  
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

  // Final thing: update everything now.
  BASS_ChannelUpdate(stream, 0);
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
  auto pianoFont = LoadSoundFont("Grand Piano.sf2");
  auto bassFont = LoadSoundFont("Colin_s_Double_Bass.sf2");
  auto drumFont = LoadSoundFont("Jazz Kit.sf2");

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

  RouteInstruments(stream);
}

static HSTREAM s_songStream = 0;

void StopMidiSong()
{
std::cout << "** Stopping MIDI song!\n";

  BASS_ChannelStop(s_songStream);
  BASS_StreamFree(s_songStream);
  s_songStream = 0;
}
 
void PlayMidiSong(const std::string& filename)
{
  if (s_songStream) 
    StopMidiSong();

std::cout << "** Play MIDI song: " << filename << "\n";

  auto sm = TheSoundManager::Instance();
  // Loading song from Glue file, or file system?
  if (auto glueFile = sm->GetGlueFile())
  {
#ifdef _DEBUG
std::cout << "BASS MIDI: using glue file.\n";
#endif
    // Find the start of the song in the glue file, and find the length
    uint32 songPos = 0;
    if (!glueFile->GetSeekBase(filename, &songPos))
    {   
      std::cout << "BASS MIDI: Song not in Glue File: " << filename << "\n";
      Assert(0);
    }   

    // Use GlueFileBinaryData to get the data without copying it
    uint32 length = glueFile->GetSize(filename);
    GlueFileBinaryData data = glueFile->GetBinary(songPos, length);

    s_songStream = BASS_MIDI_StreamCreateFile(
        TRUE, // in memory ?
        data.GetBuffer(), // start of song data 
        0, // offset
        length, // length
        BASS_SAMPLE_FLOAT, 
        1);  // sample rate
  }   
  else
  {
    s_songStream = BASS_MIDI_StreamCreateFile(
      FALSE, 
      (File::GetRoot() + filename).c_str(),
      0, 0, BASS_SAMPLE_FLOAT, 1);
  }

  if (s_songStream)
  {
    std::cout << "Loaded midi file ok?!? " << filename << "\n";
  }
  else
  {
    std::cout << "Failed to load midi file: " << filename << "\n";
    Assert(0);
  }

  MapSoundFontsToChannels(s_songStream);

  BASS_ChannelPlay(s_songStream, FALSE /* don't restart */);
}

float GetMidiSongElapsedTimeSeconds()
{
  QWORD pos = BASS_ChannelGetPosition(s_songStream, BASS_POS_BYTE); // Get current position
  double elapsedSeconds = BASS_ChannelBytes2Seconds(s_songStream, pos); // Convert to seconds
  return static_cast<float>(elapsedSeconds);
}

void MidiMutePlayerChannel(bool mute)
{
  // Mute player channel (always first channel)
  const int PLAYER_CHANNEL = 0;
  BASS_MIDI_StreamEvent(s_songStream, PLAYER_CHANNEL, MIDI_EVENT_MIXLEVEL, 
    (mute ? 0 : 1000));
}

void MidiSeek(float seconds)
{
  // Convert seconds to a byte position
  QWORD bytes = BASS_ChannelSeconds2Bytes(s_songStream, seconds);

  // Set the position
  BASS_ChannelSetPosition(s_songStream, bytes, BASS_POS_BYTE);

  RouteInstruments(s_songStream); // seeking resets the bank mappings
}
}

