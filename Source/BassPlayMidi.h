#pragma once

namespace Amju
{
// Eventually, promote this to Amjulib.
// Play individual MIDI notes and MIDI backing tracks, using BASS.
void PlayMidi(int midiNote, int velocity); //, int channel);

void PlayMidiSong(const std::string& filename);

void StopMidiSong();

float GetMidiSongElapsedTimeSeconds();

// For GSHero, call with true to mute the player channel in the backing
//  track. 
// To preview the song, including the player melody, call with false.
void MidiMutePlayerChannel(bool mute = true, int channelZB = 0);

// Seek to a position in current song -- for pause resume.
void MidiSeek(float seconds);
}

