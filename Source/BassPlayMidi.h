#pragma once

namespace Amju
{
// * BassPlayMidi *
// Eventually, promote bits of this to amjulib.
// Play individual MIDI notes and MIDI backing tracks, using BASS.

// Set up stream for player's piano. It's independent of other
//  streams, i.e. the backing tracks.
bool SetUpPlayerStream();

// Play a note on the 'player stream', separate from song.
void PlayMidi(int midiNote, int velocity); 

// * Play MIDI backing track (optionally with player's top line) *
// Convention for this game:
// Channel 1 is the player track – muted for game play, but enabled for preview.
// Channel 2: backing piano LEFT - if 2 pianos on backing track, pan them.
// Channel 3: backing piano RIGHT
// Channel 4: backing piano CENTRE - if one piano on backing track, use centre channel.
// Channel 8: backing bass 
// Channel 10: backing percussion

// Play the given MIDI song (file is loaded from filesys or glue file).
// Start the song at the given seek time. (I.e. this is position in song,
//  not a delay before we start playing.)
// If mutePlayer is true, the player melody (channel 0) is muted.
void PlayMidiSong(const std::string& filename, 
  float seekTimeSeconds = 0, bool mutePlayer = false);

// Stop the currently playing song.
void StopMidiSong();

// Play count-in MIDI song
void PlayCountIn(const std::string& filename);

// Get elapsed time in seconds that the currently playing song has
//  been playing. We sync visuals to this, not some other time measure.
float GetMidiSongElapsedTimeSeconds();

// Seek to a position in current song -- for pause resume.
void MidiSeek(float seconds);

// Call to log current song position to console; used to sanity check
//  that a song is playing, if in the awful position of not hearing
//  anything :(
void MidiLog();

bool MidiExternalConnect();

}

