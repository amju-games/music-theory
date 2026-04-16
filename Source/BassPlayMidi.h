#pragma once

namespace Amju
{
// Eventually, promote this to Amjulib.
// Play individual MIDI notes and MIDI backing tracks, using BASS.
void PlayMidi(int midiNote, int velocity); //, int channel);

// Map sound fonts to channels for this game.
//void MapSoundFontsToChannels();

void PlayMidiTrack(const std::string& filename);

}

