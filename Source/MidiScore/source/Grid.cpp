#include "Grid.h"
#include "MidiScore.h"

namespace MidiScore
{
// Change this when we know tpq
static int tpq = 480; // TODO

std::string WhitespaceForDuration(std::string e, int dur)
{
  // TODO
  return " ";

  int len = e.length();
  int desired = 8;
  return std::string(desired - len, ' ');
}

static bool IsPreamble(const Event& e)
{
  return e.IsClef() || e.IsKeySig() || e.IsTimeSig();
}

static bool NoTimeVal(const Event& e)
{
  return IsPreamble(e) || e.IsBarLine() || e.IsChordStart() || e.IsChordEnd();
}

std::string Gridify(const Events& events)
{
  std::string res;

  int bar = 1;
//  res += "// Bar " + std::to_string(bar);

  // Traverse events. Output time val when it changes.
  // Output new line when we hit a bar line.
  int prevDuration = -1;
  int numEvents = static_cast<int>(events.size());
  for (int i = 0; i < numEvents; i++)
  {
    const auto& ev = events[i];
    std::string eStr = OutputEvent(prevDuration, ev);

    res += eStr; // add string to result
    if (i == numEvents - 1) break;

    // From here, there is a next event
    const auto& next = events[i + 1];

    // Preamble stuff (generally in bar 1) should go on its own line
    //  so we don't mess up the grid. Stave, time sig, key sig, clefs.
    if (ev.IsBarLine())
    {
      res += "\n";
      res += "// Bar " + std::to_string(++bar) + "\n";
    }
    else if (IsPreamble(ev) && !IsPreamble(next))
    {
      res += "\n";
    }
    else if (NoTimeVal(ev))
    {
      // Elements with no time value should not be spaced out
      res += " ";
    }
    else
    {
      // We need to know if the next element has a time value or not.
      // TODO
      res += WhitespaceForDuration(eStr, prevDuration);
    }
  }

  return res;
}
}

