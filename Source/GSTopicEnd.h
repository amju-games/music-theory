// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <Singleton.h>
#include "GSBase3d.h"

namespace Amju
{
// * GSTopicEnd *
// Shown when we finish a topic.
// Show score, rating, give hints, etc.
class GSTopicEnd : public GSBase3d
{
public:
  GSTopicEnd();
  virtual void OnActive() override;

  void SetScore(int num, int denom);

protected:
  std::string GenerateScoreComment();

private:
  int m_num = 0;
  int m_denom = 0;
};

typedef Singleton<GSTopicEnd> TheGSTopicEnd;
}

