#pragma once

#include <ostream>
#include <string>
#include <vector>
#include "Singleton.h"

namespace Amju
{
// * HeroGameRound *
// Stores the info we need for one round of Hero mode.
struct HeroGameRound
{
  // The level this game round lives in
  int m_level = 0; 

  // Index of this round within its level
  int m_round = 0;

  // Internal (non-display) name for this round
  std::string m_name;

  // Localised title string
  std::string m_title;
  // Localised subtitle
  std::string m_subtitle;

  // Localised composer string
  std::string m_composer;

  // Filename of audio backing track
  std::string m_backingTrack;

  // Count-in: filename of audio count-in track
  std::string m_countIn;
  // Number of beats in count-in
  int m_numCountInBeats = 1;
  // Count-in GUI 
  std::string m_countInGui;

  // Filename of palette for score and keyboard
  std::string m_palette;

  // Filename of music score
  std::string m_musicScore;

  // Filename of sound font
  std::string m_soundFont;

  // Amount by which we reduce life if player makes a mistake
  int m_lifeDecrease = 1;

  // Zero-based bar numbers; the end of each bar is treated as the end of a section.
  std::vector<int> m_sectionEndBarNumbers;
};

std::ostream& operator<<(std::ostream& os, const HeroGameRound& round);

// * GameRoundManager *
// Load game round data from .csv file (call once at startup).
// Game Rounds are grouped into Levels, but I don't think we need
//  a Level type for this, do we? 
class GameRoundManager
{
public:
  bool Load();

  int GetNumGameRounds() const;

  // OK for now but we might want to get by level and index.
  const HeroGameRound& GetGameRound(int index);

  // Public for testing
  void AddGameRound(const HeroGameRound&);

private:
  std::vector<HeroGameRound> m_gameRounds;
};

using TheGameRoundManager = Singleton<GameRoundManager>;
}

