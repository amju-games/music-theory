// Bass headers are copied from 3rdPartyLibs to Source/SoundBass.
#include "../../../amjulib/Source/SoundBass/bass.h"
#include "../../../amjulib/Source/SoundBass/bassmidi.h"

#include <MessageQueue.h>
#include <SoundManager.h>
#include "BassPlayMidi.h" //???
#include "BassMidiInput.h"
#include "MusicEvent.h"

#define MIDI_INPUT_DEBUG
#define MIDI_CONNECT_DEBUG

namespace Amju
{
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
}

