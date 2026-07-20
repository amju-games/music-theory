#pragma once

#include "MidiInput.h"

namespace Amju
{
// * BassMidiInput *
// Get input from MIDI device using BASS library.
class BassMidiInput : public MidiInput
{
public:
  // Return true if device is connected; call sparingly
  bool IsConnected() const override;

  // Call to connect to midi input device
  bool Connect() override;
};

// Get the instance of the BassMidiInput class.
MidiInput& GetBassMidiInput();
}

