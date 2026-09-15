#include <array>
#include <AmjuAssert.h>
#include <EventTypes.h>

namespace Amju
{
template <size_t ARRAY_SIZE>
static bool AutoRepeatCheck(bool isDown, unsigned char key, 
  std::array<bool, ARRAY_SIZE>& flags)
{
  Assert(key < ARRAY_SIZE);
  if (isDown && !flags[key])
  {
    flags[key] = true;
    return false; // not auto-repeat
  }
  else if (!isDown)
  {
    flags[key] = false;
    return false; // not auto-repeat
  }
  else
  {
    Assert(isDown);
    Assert(flags[key]);

    // This is an auto-repeat event.
    return true;
  }
}

bool IsAutoRepeat(const KeyEvent& ke)
{
  if (ke.keyType == AMJU_KEY_CHAR)
  {
    static std::array<bool, 256> flags = {};
    return AutoRepeatCheck(ke.keyDown, static_cast<unsigned char>(ke.key), flags);
  }
  else
  {
    // 'Special key', not a character. 
    static std::array<bool, AMJU_KEY_MAX> flags = {};
    return AutoRepeatCheck(ke.keyDown, static_cast<unsigned char>(ke.keyType), flags);
  }
}
}

