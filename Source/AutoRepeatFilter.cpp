#include <array>
#include <AmjuAssert.h>
#include <EventTypes.h>

namespace Amju
{
bool IsAutoRepeat(const KeyEvent& ke)
{
  static std::array<bool, 256> autoRepeat = {};

  if (ke.keyDown && !autoRepeat[ke.key])
  {
    autoRepeat[ke.key] = true;
    return false; // not auto-repeat
  }
  else if (!ke.keyDown)
  {
    autoRepeat[ke.key] = false;
    return false; // not auto-repeat
  }
  else
  {
    Assert(ke.keyDown);
    Assert(autoRepeat[ke.key]);

    // This is an auto-repeat event.
    return true;
  }
}
}

