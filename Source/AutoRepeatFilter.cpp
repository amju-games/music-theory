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

// Key down arrays for characters and special keys.
static std::array<bool, 256> charFlags = {};
static std::array<bool, AMJU_KEY_MAX> specialFlags = {};

void ClearAutoRepeatFlags()
{
  charFlags = {};
  specialFlags = {};
}

bool IsAutoRepeat(const KeyEvent& ke)
{
  if (ke.keyType == AMJU_KEY_CHAR)
  {
    return AutoRepeatCheck(ke.keyDown, 
      static_cast<unsigned char>(ke.key), 
      charFlags);
  }
  else
  {
    // 'Special key', not a character. 
    return AutoRepeatCheck(ke.keyDown, 
      static_cast<unsigned char>(ke.keyType), 
      specialFlags);
  }
}
}

