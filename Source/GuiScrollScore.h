#pragma once

#include "GuiMusicScore.h"

namespace Amju
{
// * GuiScrollScore *
// Scroll score from right to left, rather than static/highlighting.
class GuiScrollScore : public GuiMusicScore
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  void Draw() override;
  void Update() override;
  void OnResetAnimation() override;
  bool Load(File*) override;

  // Animate: scroll the score to the left.
  // animValue: position in animation, 0..1
  // dAnimValue: difference between previous and this value of animValue.
  void AnimateSpecial(float animValue, float dAnimValue);

  // Override to build beat table after loading
  bool LoadMusicScore(File* file) override;

  // This is to 'un-hide' the parent class LoadMusicScore(const std::string&)
  //  -- why tho c++
  using GuiMusicScore::LoadMusicScore;

  // Start counting in: start scrolling the score, so that the time it
  //  takes to reach the first music event (at animTime = 0) matches
  //  the given number of beats. (We use BPM to work out the time.)
  void StartCountIn(int numCountInBeats, std::function<void()> onFinished);

  float GetCountInTimeRemaining() const;

//protected:

  // Get the last timing value passed into Animate().
  // This is used to compare player input with the beat table.
  float GetAnimTime() const;

  void AddBeatLines();

protected:
  void SendNoteEvent(const NoteEvent&) override;

  void DrawChildren() override;

protected:
  // Beats per min, used to set scroll speed.
  float m_bpm = 80.f;
  float m_scrollSpeed = 0;

  // Most recent timing value passed into Animate()
  float m_animTime = 0; 

  // Local X coord of Beat Line: this is the x-coord where the next beat should
  //  be when its time is reached.
  // We can build a vector of times (in seconds, using BPM? Or parametric/normalized?)
  //  to x-coords.
  float m_beatLineX = 0.f;
  float m_currentX = 0.f; 
  float m_nextT = 0.f; // Next time in Beat Table. When this updates, we recalc the x-velocity.

  // Count-in scroll speed and time remaining.
  float m_countInSpeed = 0;
  float m_countInTimeRemaining = 0;
  // Callback for when count-in is finished
  std::function<void()> m_countInFinishedFunc;
};
}

