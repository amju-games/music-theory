#pragma once

#include "Timeline.h"

namespace Amju
{
// * TimelineEventPlayWav *
struct TimelineEventPlayWav : public TimelineEvent
{
  static const char* NAME;

  std::string m_wavName;

  // Load wav name
  bool Load(File*) override;

  // Play the wav
  void Execute() override;
};

// * TimelineEventPlayMidiSong *
// Midi-specific because currently we have game-specific midi-song-playing.
struct TimelineEventPlayMidiSong : public TimelineEvent
{
  static const char* NAME;

  std::string m_midiSongName;

  // Load song name
  bool Load(File*) override;

  // Start playing the song
  void Execute() override;
};

// * TimelineEvent STOP MidiSong *
// Stop the currently playing song.
struct TimelineEventStopMidiSong : public TimelineEvent
{
  static const char* NAME;
  // Do we need Load?
  void Execute() override;
};
}
