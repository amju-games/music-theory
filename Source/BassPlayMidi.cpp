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
#include <MessageQueue.h>
#include <SoundManager.h>
#include "BassPlayMidi.h"
#include "MusicEvent.h"

namespace Amju
{
static const auto PIANO_FONT = "steinway_concert_piano.sf2";
static const auto BASS_FONT = "Colin_s_Double_Bass.sf2";
static const auto DRUM_FONT = "Jazz Kit.sf2";

static HSTREAM s_playerStream = 0;

void PlayMidi(int note, int velocity)
{
#ifdef PLAY_MIDI_DEBUG
  std::cout << "Playing midi note: " << note << " vel: " << velocity << "\n";
#endif

  const int pianoChannel = 0;

  BASS_MIDI_StreamEvent(
    s_playerStream, pianoChannel, MIDI_EVENT_NOTE, MAKEWORD(note, velocity));
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
#ifdef PLAY_MIDI_DEBUG
    std::cout << "Sound font already loaded and in pool: " << fontFileName << "\n";
#endif
    return it->second;
  }

  auto font = BASS_MIDI_FontInit(filename.c_str(), 0);
  if (font) 
  {
#ifdef PLAY_MIDI_DEBUG
    std::cout << "Loaded soundfont: " << fontFileName << "\n";
#endif
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
#ifdef SOUND_FONT_INFO
    if (info.name )
      std::cout << "  Name: " << info.name << "\n";

    if (info.copyright)
      std::cout << "  Copyright: " << info.copyright << "\n"; 

    if (info.comment)
      std::cout << "  Comment: " << info.comment << "\n"; 

    std::cout << "  Num presets: " << info.presets << "\n";
#endif // SOUND_FONT_INFO

    if (info.presets == 0)
    {
      std::cout << "There are no presets in this sound font: " << fontFileName << "\n";
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

void MapSoundFontsToChannelsUsingFONTEX(HSTREAM stream)
{
  // Convention for this game:
  // Channel 1 is the player track – muted for game play, but enabled for preview.
  // Channel 2: backing piano LEFT
  // Channel 3: backing piano RIGHT
  // Channel 4: backing piano CENTRE (set up the panning in code/config)
  // Channel 8: backing bass 
  // Channel 10: backing percussion
  auto pianoFont = LoadSoundFont(PIANO_FONT);
  auto bassFont = LoadSoundFont(BASS_FONT);
  auto drumFont = LoadSoundFont(DRUM_FONT);

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
static HSTREAM s_countInStream = 0;

void StopMidiSong()
{
#ifdef PLAY_MIDI_DEBUG
std::cout << "** Stopping MIDI song!\n";
#endif

  BASS_ChannelStop(s_songStream);
  BASS_StreamFree(s_songStream);
  s_songStream = 0;
}
 
// Seek to position in given stream, then redo instrument mapping etc
//  as this is trashed when we seek?!
static void MidiSeek(float seconds, HSTREAM stream)
{
  // Convert seconds to a byte position
  QWORD bytes = BASS_ChannelSeconds2Bytes(stream, seconds);

  // Set the position
  BASS_ChannelSetPosition(stream, bytes, BASS_POS_BYTE);

  RouteInstruments(stream); // seeking resets the bank mappings
  SetPanningAndReverb(stream);
}

// Common code for playing a song and playing a count-in
void LoadAndStartMidiSong(
  HSTREAM& stream, 
  const std::string& filename, 
  float seekTime, 
  bool mutePlayer, 
  float bpmMultiplier)
{ 
  auto sm = TheSoundManager::Instance();
  // Loading song from Glue file, or file system?
  if (auto glueFile = sm->GetGlueFile())
  {
#ifdef PLAY_MIDI_DEBUG
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

    stream = BASS_MIDI_StreamCreateFile(
        TRUE, // in memory ?
        data.GetBuffer(), // start of song data 
        0, // offset
        length, // length
        BASS_MIDI_NOSYSRESET, 
        44100);  // sample rate
  }   
  else
  {
    stream = BASS_MIDI_StreamCreateFile(
      FALSE, 
      (File::GetRoot() + filename).c_str(),
      0, 0, 
      BASS_MIDI_NOSYSRESET, 
      44100);
  }

  if (stream)
  {
#ifdef PLAY_MIDI_DEBUG
    std::cout << "Loaded midi file ok: " << filename << "\n";
#endif
  }
  else
  {
    std::cout << "Failed to load midi file: " << filename << "\n";
    std::cout << "Bass error code: " << BASS_ErrorGetCode() << "\n";
    Assert(0);
  }

  MapSoundFontsToChannelsUsingFONTEX(stream);

  if (mutePlayer)
  {
    const int playerTrack = 0; // player melody should be first track
    BASS_MIDI_StreamEvent(stream, playerTrack, MIDI_EVENT_MIXLEVEL, 0); 
  }

  // Set tempo multiplier
  if (bpmMultiplier > 0)
  {
    // Multiply the tempo up or down
    BASS_ChannelSetAttribute(stream, BASS_ATTRIB_MIDI_SPEED, bpmMultiplier);
  }

  MidiSeek(seekTime, stream); // RouteInstruments happens in here too.
  BASS_ChannelPlay(stream, FALSE); 
}

void PlayMidiCountIn(const std::string& filename, float bpm)
{
#ifdef PLAY_MIDI_DEBUG
std::cout << "** Play MIDI count-in: " << filename << " tempo: " << bpm << " BPM.\n";
#endif

  const float seekTime = 0;
  const bool mutePlayer = false;

  // Count-in midi files are set to 60 BPM. To get the desired tempo, mult
  //  by bpm/60. This is specific to count-in files.
  const float bpmMult = bpm / 60.f;
  LoadAndStartMidiSong(s_countInStream, filename, seekTime, mutePlayer, bpmMult);
}

void PlayMidiSong(const std::string& filename, float seekTime, bool mutePlayer)
{
#ifdef PLAY_MIDI_DEBUG
std::cout << "** Play MIDI song: " << filename << "\n";
#endif

  if (s_songStream) 
    StopMidiSong();

  const float bpm = 0; // TODO - zero means don't set
  LoadAndStartMidiSong(s_songStream, filename, seekTime, mutePlayer, bpm);
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

void MidiSongSeek(float seconds)
{
  MidiSeek(seconds, s_songStream);
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

// External MIDI event callback
static void CALLBACK BassMidiInputCallback(
  DWORD device, double time, const BYTE* buffer, DWORD length, void* user)
{
  if (length == 0) return;
  if (length == 1 && buffer[0] == 0xf8) return; // timing signal - 1 byte

  // Note on/off events
  if (length == 3 && (buffer[0] & 0xf0) == 0x90)
  {
    // Note event
    [[maybe_unused]]int channel = buffer[0] & 0x0f; // worry about that later
    BYTE midiNote = buffer[1];
    if (midiNote > 127) return;

    BYTE velocity = buffer[2];
    if (velocity > 127) return;

    bool isNoteOn = (velocity > 0); // velocity 0 means note off
    // Good news, MessageQueue::Add is thread safe
    TheMessageQueue::Instance()->Add(new MusicKbMsg(MusicKbEvent(midiNote, isNoteOn)));
  }

#ifdef MIDI_INPUT_DEBUG
  // Out of interest, print the message
  std::cout << "MIDI Data: ";
  for (DWORD i = 0; i < length; ++i)
  {
    std::cout << static_cast<int>(buffer[i]) << "  ";
  }
  std::cout << length << " bytes.\n";
#endif // MIDI_INPUT_DEBUG
}

bool BassMidiInput::Connect()
{
  BASS_MIDI_DEVICEINFO info;

  DWORD device = 0; // index of device we want
  if (!BASS_MIDI_InGetDeviceInfo(device, &info))
  {
#ifdef MIDI_CONNECT_DEBUG
    std::cout << "BASS MIDI failed to get midi device info. Error code: " << BASS_ErrorGetCode() << "\n";
#endif
    return false;
  }

  // Would be good to get this to display in setup?
  std::cout << "BASS MIDI Input device: "
    << info.name
    << "\n";

  // initialize the MIDI input device
  while (true)
  {
    bool res = BASS_MIDI_InInit(device, BassMidiInputCallback, 0);
    if (res)
    {
      break;
    }

    if (BASS_ErrorGetCode() == BASS_ERROR_ALREADY)
    {
      // Already initialised: free up and try again.
      BASS_MIDI_InFree(device);
    }
    else
    {
      std::cout << "BASS MIDI failed to initialise midi device. Error code: " << BASS_ErrorGetCode() << "\n";
      return false;
    }
  }

  if (!BASS_MIDI_InStart(device))
  {
    std::cout << "BASS MIDI failed to start recv from midi device. Error code: " << BASS_ErrorGetCode() << "\n";
    return false;
  }

  return true;
}

bool BassMidiInput::IsConnected() const
{
  BASS_MIDI_DEVICEINFO info;
  DWORD device = 0; // index of device we want -- TODO
  if (!BASS_MIDI_InGetDeviceInfo(device, &info))
  {
#ifdef MIDI_CONNECT_DEBUG
    std::cout << "BASS MIDI failed to get midi device info. Error code: " << BASS_ErrorGetCode() << "\n";
#endif
    return false;
  }

  // BASS_DEVICE_ENABLED indicates if the device is currently usable/present.
  // BASS_DEVICE_INIT indicates if you have already successfully called BASS_MIDI_InInit.
  return (info.flags & BASS_DEVICE_ENABLED);

  // TODO Also check for signals from device in callback?
}

static std::vector<std::string> GetTrackNames(HSTREAM stream) 
{
  std::vector<std::string> res;
  // Attempt to get the number of track mark events in the MIDI stream
  int markCount = BASS_MIDI_StreamGetMarks(stream, -1, BASS_MIDI_MARK_TRACK, NULL);
    
  if (markCount > 0) 
  {
    // Create a vector to hold all the markers
    std::vector<BASS_MIDI_MARK> marks(markCount);
        
    // Fetch the actual markers
    BASS_MIDI_StreamGetMarks(stream, -1, BASS_MIDI_MARK_TRACK, marks.data());
        
    for (int i = 0; i < markCount; i++) 
    {
      // The 'text' field of the BASS_MIDI_MARK struct contains the track name
      res.push_back(marks[i].text);
    }
  } 
  return res;
}

std::vector<std::string> GetPlayingSongTrackNames()
{
  return GetTrackNames(s_songStream);
}

static DWORD GetMidiTrackCount(HSTREAM midiStream) 
{
    DWORD track = 0;
    while (true) {
        // Passing NULL to the event array parameter simply checks if the track exists
        DWORD eventCount = BASS_MIDI_StreamGetEvents(midiStream, track, 0, NULL);
        
        // If BASS returns -1, it means the track index is out of bounds
        if (eventCount == (DWORD)-1) {
            break;
        }
        track++;
    }
    return track; // This is your total track count
}

static void ApplyTrackMuteSolo(HSTREAM midiStream, 
  const std::set<int>& muteTracks, 
  const std::set<int>& soloTracks) 
{
  // Determine total tracks inside the file
  DWORD totalTracks = GetMidiTrackCount(midiStream);

  for (DWORD i = 0; i < totalTracks; ++i) 
  {
    bool shouldMute = false;

    // Rule 1: If a Solo list exists, everything NOT in that list must be muted
    if (!soloTracks.empty()) 
    {
        if (soloTracks.find(i) == soloTracks.end()) 
        {
            shouldMute = true;
        }
    }
        
    // Rule 2: If it's explicitly in the Mute list, it must be muted
    if (muteTracks.find(i) != muteTracks.end()) 
    {
        shouldMute = true;
    }

    // Apply muting to the specific track index via the BASS_MIDI_StreamEvent API
    // Passing 0 mutes the track completely, passing -1 restores default track processing
    int mixValue = shouldMute ? 0 : -1;
    BASS_MIDI_StreamEvent(midiStream, i, MIDI_EVENT_MIXLEVEL, mixValue);
  }
  BASS_ChannelPlay(midiStream, FALSE); 
}

void ApplyMuteSoloToPlayingSong(
  const std::set<int>& muteTracks, 
  const std::set<int>& soloTracks) 
{
  ApplyTrackMuteSolo(s_songStream, muteTracks, soloTracks);
}
}
