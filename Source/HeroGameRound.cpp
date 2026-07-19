//#define GAME_ROUND_DEBUG

#ifdef GAME_ROUND_DEBUG
#include <iostream>
#endif

#include <File.h>
#include <Localise.h>
#include <StringUtils.h>
#include "HeroGameRound.h"

namespace Amju
{
void GameRoundManager::AddGameRound(const HeroGameRound& round)
{
  m_gameRounds.emplace_back(round);
}

bool GameRoundManager::Load()
{
  m_gameRounds.clear();

  // Load .csv file
  File f(File::NO_VERSION); // .csv file doesn't have version number at top
  if (!f.OpenRead("Songs/songs.csv"))
  {
    return false;
  }
  // Identify lines we don't want to include by counting the number of cells.
  const int EXPECTED_NUM_CELLS = 14;
  std::string line;
  bool isFirstLine = true;
  while (f.GetDataLine(&line))
  {
    auto cells = Split(line, '\t');
    
    if (cells.size() != EXPECTED_NUM_CELLS)
    {
      std::stringstream ss;
      ss << "Ignoring line, it has " << cells.size() << " cells!";
      f.ReportError(ss.str());
      continue;
    }

    // Ignore the first line, it's column headings
    if (isFirstLine)
    {
      isFirstLine = false;
      continue;
    }

    HeroGameRound round;
    round.m_level = ToInt(cells[0]);
    round.m_round = ToInt(cells[1]);
    round.m_name = cells[2];
    round.m_title = Lookup(cells[3]); // Get localised string
    if (!cells[4].empty())
    {
      round.m_subtitle = Lookup(cells[4]); // Get localised string
    }
    round.m_composer = Lookup(cells[5]); // Get localised string
    round.m_backingTrack = "Songs/" + cells[6];
    round.m_countIn = "Songs/" + cells[7];
    round.m_numCountInBeats = ToInt(cells[8]);
    round.m_countInGui = "Gui/" + cells[9];
    round.m_palette = "Image/" + cells[10] + ".pal"; // palette resource
    round.m_musicScore = "Songs/" + cells[11];
    round.m_lifeDecrease = ToInt(cells[12]);
    // Zero-based bar numbers for ends of sections.
    // If none specified, the cell could be empty, or have, -1 as the content.
    const auto cell = cells[13];
    if (!cell.empty() && ToInt(cell) > 0)
    {
      round.m_sectionEndBarNumbers = SplitIntoVec(cell);
    }

    AddGameRound(round);
  }

  if (m_gameRounds.empty()) return false;
#ifdef GAME_ROUND_DEBUG
std::cout << "Loaded game round .csv file ok! There are: " << m_gameRounds.size() << " rounds.\n";
#endif
  return true;
}

std::ostream& operator<<(std::ostream& os, const HeroGameRound& round)
{
  return os 
    << round.m_level << ", "
    << round.m_round << ", "
    << round.m_name << ", "
    << round.m_title << ", "
    << round.m_composer << ", "
    << round.m_backingTrack << ", "
    << round.m_countIn << ", "
    << round.m_numCountInBeats << ", "
    << round.m_palette << ", "
    << round.m_musicScore << ", "
    << round.m_lifeDecrease  << ", "
    << round.m_sectionEndBarNumbers.size() << " section ends, "
    << ".\n";
}

int GameRoundManager::GetNumGameRounds() const
{
  return static_cast<int>(m_gameRounds.size());
}

const HeroGameRound& GameRoundManager::GetGameRound(int index)
{
  return m_gameRounds[index];
}
}

