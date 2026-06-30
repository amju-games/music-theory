// * PIANO FEST *
// (c) Copyright 2026 Juliet Colman

#pragma once

// TODO Factor different resposiblities out of GuiMusicScore!
#include "GuiMusicScore.h" // Beat and BeatVec
#include "NoteEvent.h"

namespace Amju
{
static const float HUGELY_LONG_TIME = 3600.f;

// * Resumer *
// Finds the best place to resume the game after a pause.
class Resumer
{
public:
// Given the song time at which the game round was paused, return  
//  an appropriate resume time. 
// To do this, we need info about the beats and bars in the song. We 
//  want to go back to the first beat in the current bar, or the first
//  beat of an earlier bar if the current bar isn't suitable. E.g. a 
//  tied note.
// Returns HUGELY_LONG_TIME to trigger the 'game won' state if the
//  resume time falls at the end of the beats.
// Returns 0 if there is no suitable resume point (so the round restarts).
static float FindResumePoint(
  float pauseTime, 
  const BeatVec& beats, 
  const NoteEvents& noteEvents);

// Look for a note event at the given time; return true if found.
// This is used to check if there is a note event at the time
//  of the first beat of the bar. If not, we go back to the prev bar.
static bool NoteEventWasFoundAtTime(float time, const NoteEvents& noteEvents);

// `iter` points to a beat in the given BeatVec for the piece.
// Decrement iter until we point to the first beat in a bar.
// Set `time` to the time of the beat iter points to.
static void GoToFirstBeatOfBar(
  const BeatVec& beats, BeatVec::const_iterator iter, float& time);

};
}

