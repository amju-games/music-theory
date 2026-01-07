#pragma once

#include <ostream>
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

  // TODO Maybe learning outcomes?

  // Localised title string
  std::string m_title;

  // Filename of audio backing track
  std::string m_backingTrack;

  // Count-in: filename of audio count-in track
  std::string m_countIn;
  // Number of beats in count-in
  int m_numCountInBeats = 1;

  // Filename of palette for score and keyboard
  std::string m_palette;

  // Filename of music score
  std::string m_musicScore;
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

private:
  std::vector<HeroGameRound> m_gameRounds;
};

using TheGameRoundManager = Singleton<GameRoundManager>;
}

