// * Amjula music theory * (c) Copyright 2024 Juliet Colman

#pragma once

// * LevelManager *
// Encapsulate responsibility for tracking/peristing the current level num.
namespace Amju
{
class ILevelManager
{
public:
  virtual ~ILevelManager() = default;

  // Level num: zero based index
  virtual int GetLevelNum() const = 0;

  // Set the new level num; persist this value
  virtual void SetLevelNum(int level) = 0;

  virtual int GetNumberOfLevels() const = 0;
};

ILevelManager& GetLevelManager();

class LevelManager : public ILevelManager
{
public:
  LevelManager();
  int GetLevelNum() const override;
  void SetLevelNum(int level) override;
  int GetNumberOfLevels() const override;

private:
  int m_levelNum = 0;
};

// Only required for setting a mock for testing
void SetLevelManager(ILevelManager&);

}

