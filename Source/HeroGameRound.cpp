#include <sstream>
#include <File.h>
#include <Localise.h>
#include <StringUtils.h>
#include "HeroGameRound.h"

namespace Amju
{
bool GameRoundManager::Load()
{
  m_gameRounds.clear();

  // Load .csv file
  File f(File::NO_VERSION); // .csv file doesn't have version number at top
  if (!f.OpenRead("hero-game-rounds.csv"))
  {
    return false;
  }
  // Identify lines we don't want to include by counting the number of cells.
  const int EXPECTED_NUM_CELLS = 11;
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

    for (const auto& cell : cells)
    {
      if (cell.empty())
      {
        f.ReportError("Bad game round data, some fields are blank.");
        return false;
      }   
    }

    HeroGameRound round;
    round.m_level = ToInt(cells[0]);
    round.m_round = ToInt(cells[1]);
    round.m_name = cells[2];
    round.m_title = Lookup(cells[3]); // Get localised string
    round.m_composer = Lookup(cells[4]); // Get localised string
    round.m_backingTrack = "Songs/" + cells[5];
    round.m_countIn = "Songs/" + cells[6];
    round.m_numCountInBeats = ToInt(cells[7]);
    // TODO This is going to change to a list of midi pitches we want
    //       to colour in 
    round.m_palette = "Gui/Palettes/" + cells[8];
    round.m_musicScore = "Songs/" + cells[9];
    round.m_soundFont = "Sound/" + cells[10];
std::cout << round;

    m_gameRounds.push_back(round);
  }

  if (m_gameRounds.empty()) return false;
  
std::cout << "Loaded game round .csv file ok! There are: " << m_gameRounds.size() << " rounds.\n";
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
    << round.m_soundFont << ".\n";
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

