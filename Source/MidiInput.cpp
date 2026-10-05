#include "MidiInput.h"

namespace Amju
{
static RCPtr<MidiInput> s_midiInput;

MidiInput* GetMidiInput()
{
  return s_midiInput;
}

void SetMidiInput(RCPtr<MidiInput> midiInput)
{
  s_midiInput = midiInput;
}
}
