#pragma once

namespace Amju
{
// * MidiInput *
// Interface for MIDI input types.
class MidiInput
{
public:
  virtual ~MidiInput() = default;
  virtual bool IsConnected() const = 0;
  virtual bool Connect() = 0;
};
}

