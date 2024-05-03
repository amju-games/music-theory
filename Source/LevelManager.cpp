// * Amjula music theory * (c) Copyright 2024 Juliet Colman

#include "LevelManager.h"
#include "UserProfile.h"

namespace Amju
{
ILevelManager& GetLevelManager()
{
  static LevelManager lm;
  return lm;
}

static const char* const LEVEL_KEY = "level";

LevelManager::LevelManager()
{
  m_levelNum = TheUserProfile()->GetCurrentLevel();
}

int LevelManager::GetLevelNum() const
{
  return m_levelNum;
}

void LevelManager::SetLevelNum(int level)
{
  if (level == m_levelNum)
  {
    return;
  }
  m_levelNum = level;
  TheUserProfile()->SetCurrentLevel(m_levelNum);
}

int LevelManager::GetNumberOfLevels() const
{
  return 4; // TODO!!!!!
}
}