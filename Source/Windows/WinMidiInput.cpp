#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <MessageQueue.h>
#include "MusicEvent.h"
#include "WinMidiInput.h"

#pragma comment(lib, "winmm.lib")

namespace Amju
{
static const BYTE NOTE_ON = 0x90;
static const BYTE NOTE_OFF = 0x80;

static void OnMidiNote(BYTE command, BYTE channel, BYTE data1, BYTE data2)
{
  if (command == 0x90 && data2 > 0)
  {
    std::cout << "Note On  -> Channel: " << (int)channel + 1
      << " | Note: " << (int)data1
      << " | Velocity: " << (int)data2 << "\n";

    // Good news, MessageQueue::Add is thread safe
    TheMessageQueue::Instance()->Add(
      new MusicKbMsg(MusicKbEvent(data1, data2)));
  }
  else if (command == 0x80 || (command == 0x90 && data2 == 0))
  {
    std::cout << "Note Off -> Channel: " << (int)channel + 1
      << " | Note: " << (int)data1 << "\n";

    TheMessageQueue::Instance()->Add(
      new MusicKbMsg(MusicKbEvent(data1, 0)));
  }
}

static void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
  switch (wMsg)
  {
  case MIM_DATA:
  {
    // Extract the bytes from the packed 32-bit DWORD
    const BYTE status = LOBYTE(LOWORD(dwParam1));
    const BYTE data1 = HIBYTE(LOWORD(dwParam1));
    const BYTE data2 = LOBYTE(HIWORD(dwParam1));
    //DWORD timestamp = dwParam2; // Timestamp in milliseconds since midiInStart

    // Simple parse example
    BYTE command = status & 0xF0;
    BYTE channel = status & 0x0F;
    if (command == NOTE_ON || command == NOTE_OFF)
    {
      OnMidiNote(command, channel, data1, data2);
    }
    break;
  }
  case MIM_OPEN:
    std::cout << "MIDI Device opened successfully.\n"; //
    break;
  case MIM_CLOSE:
    std::cout << "MIDI Device closed.\n"; //
    break;
  }
}

static std::vector<std::wstring> s_deviceNames;

struct ConnectedDevice : public RefCounted
{
  std::wstring name;
  int id = 0;
  HMIDIIN hMidiIn = nullptr;
  
  ConnectedDevice(int device)
  {
    id = device;
    Connect(device);
  }

  ~ConnectedDevice()
  {
    Disconnect();
  }

  bool Connect(int device)
  {
    // Open the MIDI device
    MMRESULT result = midiInOpen(
      &hMidiIn, device, (DWORD_PTR)MidiInProc, 0, CALLBACK_FUNCTION);
    if (result != MMSYSERR_NOERROR) 
    {
      std::cerr << "Failed to open MIDI device. Error code: " << result << "\n";
      return false;
    }

    if (device >= static_cast<int>(s_deviceNames.size()))
    {
      std::cerr << "MIDI logic error: device ID too big.\n";
      return false;
    }

    name = s_deviceNames[device];

    std::wcout << L"Connecting to: " << name << L"\n";

    // Start receiving messages
    midiInStart(hMidiIn); 
    return true;
  }

  void Disconnect()
  {
    // Clean up
    if (hMidiIn != nullptr)
    {
      std::wcout << L"Disconnecting: " << name << L"\n";

      midiInStop(hMidiIn);  // Stop recording
      midiInClose(hMidiIn); // Free device resource
    }
  }
};

static std::vector<RCPtr<ConnectedDevice>> s_connectedDevices;

int FindMidiDevices()
{
  s_deviceNames.clear();
  UINT numDevs = midiInGetNumDevs(); 
  if (numDevs == 0) 
  {
    std::cout << "No MIDI input devices found.\n";
    return 0;
  }

  MIDIINCAPS caps; 
  for (UINT i = 0; i < numDevs; ++i) 
  {
    if (midiInGetDevCaps(i, &caps, sizeof(MIDIINCAPS)) == MMSYSERR_NOERROR)
    {
      std::wcout << L"Device ID " << i << L": " << caps.szPname << std::endl;
      s_deviceNames.push_back(caps.szPname);
    }
  }
  return numDevs;
}

bool WinMidiInput::IsConnected() const
{
  return (midiInGetNumDevs() > 0);
}

bool WinMidiInput::Connect()
{
  s_connectedDevices.clear();

  int numDevices = FindMidiDevices();
  if (numDevices == 0) return false;

  for (int i = 0; i < numDevices; ++i)
  {
    s_connectedDevices.push_back(new ConnectedDevice(i));
  }

  return true;
}

void WinMidiInput::OnDeviceChange()
{
  Connect();
}
}
