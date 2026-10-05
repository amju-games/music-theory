// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

#pragma once

#include <RCPtr.h>

namespace Amju
{
// * MidiInput *
// Interface for MIDI input types.
class MidiInput : public RefCounted
{
public:
  virtual ~MidiInput() = default;
  virtual int GetNumConnections() const = 0;
  virtual bool Connect() = 0;
  virtual void OnDeviceChange() = 0;
};

// Get the instance of the BassMidiInput class.
MidiInput* GetMidiInput();

void SetMidiInput(RCPtr<MidiInput> midiInput);
}
