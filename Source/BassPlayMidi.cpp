// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

// Implement PlayMidi() using the BASS library.

#include <iostream>
#include <unordered_map>

// TODO Just for dev - then move this to BassSoundPlayer...?

// Bass headers are copied from 3rdPartyLibs to Source/SoundBass.
#include "../../../amjulib/Source/SoundBass/bass.h"
#include "../../../amjulib/Source/SoundBass/bassmidi.h"

#include <Directory.h>
#include <File.h>
#include <GlueFile.h>
#include <SoundManager.h>
#include "BassPlayMidi.h"

namespace Amju
{
static HSTREAM s_playerStream = 0;

void PlayMidi(int note, int velocity)
{
#ifdef PLAY_MIDI_DEBUG
  std::cout << "Playing midi note: " << note << " vel: " << velocity << "\n";
#endif

  BASS_MIDI_StreamEvent(s_playerStream, 0, MIDI_EVENT_NOTE, MAKEWORD(note, velocity));
}

HSOUNDFONT LoadSoundFont(const std::string fontFileName)
{
#ifdef AMJU_IOS
  // Assets are not in subdirectories on iOS
  std::string prefix = File::GetRoot();
#else
  std::string prefix = File::GetRoot() + "Sound/";
#endif

  auto filename = prefix + fontFileName;

  // Use this map so we only ever load a soundfont once.
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

  // Get additional info
  BASS_MIDI_FONTINFO info;
  if (BASS_MIDI_FontGetInfo(font, &info)) 
  {
    std::cout << "  Name: " << info.name << "\n";
    std::cout << "  Copyright: " << info.copyright << "\n"; 
    std::cout << "  Comment: " << info.comment << "\n"; 
    std::cout << "  Num presets: " << info.presets << "\n";
    if (info.presets == 0)
    {
      std::cout << "There are no presets in this sound font!\n";
      Assert(0);
    }
  }
  else 
  {
    std::cout << "Sound Font Info Error: " << BASS_ErrorGetCode() << "\n";
    Assert(0);
  }

  fontPool[filename] = font; // store for next time
  return font;
}

void RouteBass(HSTREAM stream)
{
  // Route Bass (Ch 8 / Index 7) back to Bank 1
  BASS_MIDI_StreamEvent(stream, 7, MIDI_EVENT_BANK, 1);
  
  // Force Channel 8 to play the Acoustic Bass preset (32)
  // Just in case the MIDI file sends a program change that messes it up
  BASS_MIDI_StreamEvent(stream, 7, MIDI_EVENT_PROGRAM, 32);
}

void RouteDrums(HSTREAM stream)
{
  // Force Channel 10 (Index 9) to use Drum Logic
  BASS_MIDI_StreamEvent(stream, 9, MIDI_EVENT_DRUMS, 1);

  // Route Drums (Ch 10 / Index 9) back to Bank 128
  BASS_MIDI_StreamEvent(stream, 9, MIDI_EVENT_BANK, 128);
  
  // Re-force the Program (Instrument)
  // This ensures they don't default to Piano (0) if the seek
  // landed exactly after a Program Change event.
  BASS_MIDI_StreamEvent(stream, 9, MIDI_EVENT_PROGRAM, 0);
}

void RouteInstruments(HSTREAM stream)
{
  // Apply our routing so channel 8 is bass, channel 10 is drums.
  RouteBass(stream);
  RouteDrums(stream);
}

void SetPanningAndReverb(HSTREAM stream)
{
  // ** Panning **
  // Channel 2 (Index 1) -> Hard Left
  BASS_MIDI_StreamEvent(stream, 1, MIDI_EVENT_PAN, 0); 
  // Channel 3 (Index 2) -> Hard Right
  BASS_MIDI_StreamEvent(stream, 2, MIDI_EVENT_PAN, 127);
  // Channel 8 (Index 7) -> Center
  BASS_MIDI_StreamEvent(stream, 7, MIDI_EVENT_PAN, 64);
  
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
  auto pianoFont = LoadSoundFont("Grand Piano.sf2");
  auto bassFont = LoadSoundFont("Colin_s_Double_Bass.sf2");
  auto drumFont = LoadSoundFont("Jazz Kit.sf2");

  BASS_MIDI_FONT fonts[3]; // Piano, Bass, and now Drums

  // 1. Piano (Bank 0)
  fonts[0].font = pianoFont;
  fonts[0].preset = -1; 
  fonts[0].bank = 0;    

  // 2. Bass (Bank 1)
  fonts[1].font = bassFont;
  fonts[1].preset = -1; 
  fonts[1].bank = 1;    

  // 3. Drums (Bank 128) 
  // Even if your font only has one drum kit, Bank 128 is the MIDI standard for drums.
  fonts[2].font = drumFont;
  fonts[2].preset = -1; 
  fonts[2].bank = 128; 

  BASS_MIDI_StreamSetFonts(stream, fonts, 3);
}

void MapSoundFontsToChannelsUsingFONTEX(HSTREAM stream)
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

  // 1. Create your font array (Extended version)
  BASS_MIDI_FONTEX fonts[3] = {{0}};
  
  // PIANO: Use all presets from the piano font, map them to Bank 0
  fonts[0].font = pianoFont;
  fonts[0].spreset = -1; fonts[0].sbank = -1; fonts[0].dpreset = -1;
  fonts[0].dbank = 0;
  fonts[0].dbanklsb = 0;
 
  // BASS: Use all presets from the bass font, map them to Bank 1
  fonts[1].font = bassFont;
  fonts[1].spreset = -1; fonts[1].sbank = -1; fonts[1].dpreset = -1;
  fonts[1].dbank = 1;
  fonts[1].dbanklsb = 0;  

  // DRUMS: Use all presets from the drum font, map them to Bank 128
  fonts[2].font = drumFont;
  fonts[2].spreset = -1; fonts[2].sbank = -1; fonts[2].dpreset = -1;
  fonts[2].dbank = 128;
  fonts[2].dbanklsb = 0;  
  
  // 2. Apply to your stream with the EX flag
  BASS_MIDI_StreamSetFonts(stream, fonts, 3 | BASS_MIDI_FONT_EX);
}

static HSTREAM s_songStream = 0;

void StopMidiSong()
{
std::cout << "** Stopping MIDI song!\n";

  BASS_ChannelStop(s_songStream);
  BASS_StreamFree(s_songStream);
  s_songStream = 0;
}
 
void PlayMidiSong(const std::string& filename, float seekTime, bool mutePlayer)
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
        BASS_MIDI_NOSYSRESET, 
        44100);  // sample rate
  }   
  else
  {
    s_songStream = BASS_MIDI_StreamCreateFile(
      FALSE, 
      (File::GetRoot() + filename).c_str(),
      0, 0, 
      BASS_MIDI_NOSYSRESET, 
      44100);
  }

  if (s_songStream)
  {
    std::cout << "Loaded midi file ok: " << filename << "\n";
  }
  else
  {
    std::cout << "Failed to load midi file: " << filename << "\n";
    Assert(0);
  }

  MapSoundFontsToChannelsUsingFONTEX(s_songStream);
  if (mutePlayer)
  {
    const int playerTrack = 0; // player melody should be first track
    BASS_MIDI_StreamEvent(s_songStream, playerTrack, MIDI_EVENT_MIXLEVEL, 0); 
  }
  MidiSeek(seekTime); // RouteInstruments happens in here too.
  BASS_ChannelPlay(s_songStream, FALSE); 
}

void MidiLog()
{
  // Writes to console - call every frame to check if MIDI stream is running.
  BOOL isActive = BASS_ChannelIsActive(s_songStream);
  QWORD pos = BASS_ChannelGetPosition(s_songStream, BASS_POS_BYTE);
  std::cout << "MIDI: " <<  (isActive ? "active" : " NOT ACTIVE") << "  pos: " << pos << "\n";
}

float GetMidiSongElapsedTimeSeconds()
{
  QWORD pos = BASS_ChannelGetPosition(s_songStream, BASS_POS_BYTE); // Get current position
  double elapsedSeconds = BASS_ChannelBytes2Seconds(s_songStream, pos); // Convert to seconds
  return static_cast<float>(elapsedSeconds);
}

void MidiSeek(float seconds)
{
  // Convert seconds to a byte position
  QWORD bytes = BASS_ChannelSeconds2Bytes(s_songStream, seconds);

  // Set the position
  BASS_ChannelSetPosition(s_songStream, bytes, BASS_POS_BYTE);

  RouteInstruments(s_songStream); // seeking resets the bank mappings
  SetPanningAndReverb(s_songStream);
}

bool SetUpPlayerStream()
{
  const int numMidiChannels = 16; // ?
  s_playerStream = BASS_MIDI_StreamCreate(
    numMidiChannels, 
    BASS_SAMPLE_FLOAT, 
    0); 

  if (s_playerStream == 0)
  {
    std::cout << "BASS error code: " << BASS_ErrorGetCode() << "\n";
    return false;
  }

#ifdef MACOSX
  // Attempting to reduce latency...
  BASS_ChannelSetAttribute(s_playerStream, BASS_ATTRIB_NOBUFFER, 1); 
#endif

  // Use same piano sound font as backing tracks. (Do we want this?)
  MapSoundFontsToChannelsUsingFONTEX(s_playerStream);

  // Do we want to add reverb to match backing tracks?
  // Panning: should be centred.
  SetPanningAndReverb(s_playerStream);

  BASS_ChannelPlay(s_playerStream, FALSE);

  return true;
}
}

