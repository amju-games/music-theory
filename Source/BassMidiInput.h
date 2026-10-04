#pragma once

#include "MidiInput.h"

namespace Amju
{
// * BassMidiInput *
// Get input from MIDI device using BASS library.
class BassMidiInput : public MidiInput
{
public:
  BassMidiInput();

  // Return true if device is connected; call sparingly
  int GetNumConnections() const override;

  // Call to connect to midi input device
  bool Connect() override;

  void OnDeviceChange() override;
};
}

