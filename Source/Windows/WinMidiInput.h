#pragma once

#include "MidiInput.h"

namespace Amju
{
// * WinMidiInput *
// MIDI input implemented with Windows MM library
class WinMidiInput : public MidiInput
{
public:
  int GetNumConnections() const override;
  bool Connect() override;
  void OnDeviceChange() override;
};
}
