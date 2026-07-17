#include "AIWait.h"

namespace Amju
{
const char* AIWait::NAME = "wait";

AIWait::AIWait(float maxTime) 
{
  m_maxTime = maxTime;
  m_rank = -100.0f;
}

const char* AIWait::GetName() const
{
  return NAME;
}
}
