// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

// Bass headers are copied from 3rdPartyLibs to Source/SoundBass.
#include "../../../amjulib/Source/SoundBass/bass.h"
#include "../../../amjulib/Source/SoundBass/bassmidi.h"

#include <iostream>
#include <unordered_map>
#include "BassErrorStrings.h"

namespace Amju
{
std::string BassVersion(uint32_t version)
{
  int major    = (version >> 24) & 0xFF;
  int minor    = (version >> 16) & 0xFF;
  int revision = (version >> 8)  & 0xFF;
  int build    =  version        & 0xFF;

  return std::to_string(major) + "." + 
         std::to_string(minor) + "." + 
         std::to_string(revision) + "." + 
         std::to_string(build); 
}

std::string GetBassErrorString(int errorCode) 
{
    // Declared static so it initializes only ONCE for performance
    static const std::unordered_map<int, std::string> errorMap = {
        // Core Error Codes
        { BASS_OK,                  "No error" },
        { BASS_ERROR_MEM,           "Memory error / Out of memory" },
        { BASS_ERROR_FILEOPEN,      "Can't open the file" },
        { BASS_ERROR_DRIVER,        "Can't find a free/valid driver" },
        { BASS_ERROR_BUFLOST,       "The sample buffer was lost" },
        { BASS_ERROR_HANDLE,        "Invalid handle" },
        { BASS_ERROR_FORMAT,        "Unsupported sample format" },
        { BASS_ERROR_POSITION,      "Invalid playback position" },
        { BASS_ERROR_INIT,          "BASS_Init has not been called" },
        { BASS_ERROR_START,         "BASS_Start has not been called" },
        { BASS_ERROR_SSL,           "SSL/HTTPS support is not available" },
        { BASS_ERROR_ALREADY,       "Already initialized/paused/started" },
        { BASS_ERROR_NOTAUDIO,      "The file does not contain audio data" },
        { BASS_ERROR_NOCHAN,        "Can't get a free channel" },
        { BASS_ERROR_ILLTYPE,       "An illegal type was specified" },
        { BASS_ERROR_ILLPARAM,      "An illegal parameter was specified" },
        { BASS_ERROR_NO3D,          "No 3D support" },
        { BASS_ERROR_NOEAX,         "No EAX support" },
        { BASS_ERROR_DEVICE,        "Illegal device number" },
        { BASS_ERROR_NOPLAY,        "Not playing" },
        { BASS_ERROR_FREQ,          "Illegal sample rate" },
        { BASS_ERROR_NOTAVAIL,      "The requested storage/action is not available" },
        { BASS_ERROR_DECODE,        "The channel is a decoding channel" },
        { BASS_ERROR_EMPTY,         "A DX8 effect was requested on an empty channel" },
        { BASS_ERROR_NONET,         "No internet connection could be opened" },
        { BASS_ERROR_CREATE,        "Couldn't create the file" },
        { BASS_ERROR_NOFX,          "Effects are not enabled on this channel" },
        { BASS_ERROR_NOTFILE,       "The stream is not a file stream" },
        { BASS_ERROR_PROTOCOL,      "Unsupported internet protocol" },
        { BASS_ERROR_TIMEOUT,       "The request timed out" },
        { BASS_ERROR_FILEFORM,   "Unknown/unsupported file format" },
        { BASS_ERROR_BUSY,          "The device or resource is busy" },
        { BASS_ERROR_CODEC,         "A required codec is missing" },
        { BASS_ERROR_ENDED,         "The channel/file has ended" },
        { BASS_ERROR_SPEAKER,       "The device does not support the requested speaker assignment" },
        { BASS_ERROR_VERSION,       "The loaded DLL is the wrong version" },
        { BASS_ERROR_DX,            "The requested DirectX version is not installed" },
        { BASS_ERROR_TIMEOUT,       "The connection timed out" },
        { BASS_ERROR_UNKNOWN,       "Unknown error" },

        // BASSMIDI Native Custom Code
        { BASS_ERROR_MIDI_INCLUDE,        "No SoundFont has been loaded/assigned to play this MIDI stream" },

        // Core codes that mean specific things when called from BASSMIDI functions
        { BASS_ERROR_DEVICE,        "Illegal device number (MIDI input device is invalid)" },
        { BASS_ERROR_INIT,          "BASS_Init has not been called (Required to decode SoundFont samples)" },
        { BASS_ERROR_NOTAVAIL,      "Action unavailable (e.g., SoundFont is not packed, or autofree flag misused)" },
        { BASS_ERROR_CODEC,         "A required codec is missing to decode packed SoundFont samples" },
        { BASS_ERROR_ILLPARAM,      "An illegal parameter was specified (or invalid SoundFont handle array)" },
    };

    auto it = errorMap.find(errorCode);
    if (it != errorMap.end()) {
        return it->second;
    }

    return "Unrecognised Error Code (" + std::to_string(errorCode) + ")";
}

int CheckBassStatus() 
{
    int errorCode = BASS_ErrorGetCode();
    if (errorCode != BASS_OK) 
    {
        std::cerr << "BASS Error: \"" 
          << GetBassErrorString(errorCode) 
          << "\" (error code "
          << errorCode 
          << ")\n";
    }
    return errorCode;
}
}

