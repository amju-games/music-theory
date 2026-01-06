#pragma once

#include "GuiMusicScore.h"

namespace Amju
{
// Beat table: map time (normalized) -> x coord in score (local coord)
// E.g. t=0 -> x=0; t=0.1 -> x=0.2; t=0.2 -> x=0.5, ...
// We can't rely on equal spacing and keep the scroll speed constant - 
//  different glyphs take up different widths.
using BeatTable = std::map<float, float>;

// * GuiScrollScore *
// Scroll score from right to left, rather than static/highlighting.
class GuiScrollScore : public GuiMusicScore
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  void Draw() override;
  void Update() override;
  void Animate(float animValue) override;
  void OnResetAnimation() override;
  bool Load(File*) override;

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
  // Build table of Time (normalized) -> x-coord (local coords).
  // Public and with BeatTable param for unit testing.
  void BuildBeatTable(BeatTable&);

  const BeatTable& GetBeatTable() const;

  // Get the last timing value passed into Animate().
  // This is used to compare player input with the beat table.
  float GetAnimTime() const;

  void AddBeatLines();

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

  BeatTable m_beatTable;
};
}

