#include "Staccato.h"

namespace MidiScore
{
int StaccatoPass(Events& events)
{
  // This pass happens early on. We should just have note events.
  // Look for gaps between this event and the next. If the
  //  duration is the same, double the note duration and flag as
  //  staccato.

  int res = 0;
  int n = static_cast<int>(events.size());
  for (int i = 0; i < n - 1; i++)
  {
    Event& e = events[i];
    if (!e.IsNote()) continue;

    const Event& next = events[i + 1];
    if (!next.IsNote()) continue;
    
    assert(e.m_end > 0);
    int gap = next.m_start - e.m_end;
    if (gap == e.m_duration)
    {
      e.m_end += e.m_duration;
      e.m_duration += e.m_duration;
      // TODO Recalc time val properly?
      // Double timeval (next timeval is double). Dots stay as they are.
      e.m_timeVal = TimeVal(static_cast<int>(e.m_timeVal) + 1); 
      e.m_isStacc = true;
      ++res;
    }
  }

  return res;
}
}

