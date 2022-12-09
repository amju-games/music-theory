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

protected:
  std::string GenerateScoreComment();
};

typedef Singleton<GSTopicEnd> TheGSTopicEnd;
}

