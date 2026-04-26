#pragma once

#include "Timeline.h"

namespace Amju
{
struct TimelineEventPlayWav : public TimelineEvent
{
  static const char* NAME;

  std::string m_wavName;

  // Load wav name
  bool Load(File*) override;

  // Play the wav
  void Execute() override;
};
}
