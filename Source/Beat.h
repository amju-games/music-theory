// * PIANO FEST *
// (c) Copyright 2026 Juliet Colman

#pragma once

#include <vector>

namespace Amju
{
// * Beat *
// Normalised time for a beat in a piece of music.
// Used for a musical score displayed by GuiMusicScore.
struct Beat
{
  float m_time = 0; // (normalised, i.e. 0..1)
  int m_bar = 1; // bar number (1-based)
  int m_beat = 1; // beat number in bar (1-based)

  Beat(float time, int bar, int beat) : 
    m_time(time), m_bar(bar), m_beat(beat) {}

  bool operator<(const Beat& b) { return m_time < b.m_time; }
};

using BeatVec = std::vector<Beat>;
}

