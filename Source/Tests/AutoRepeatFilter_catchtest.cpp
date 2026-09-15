#include "AutoRepeatFilter.h"
#include "catch.hpp"
#include "KeyInputHandler.h" // for MakeKeyEvent convenience functions

using namespace Amju;

TEST_CASE("Auto-repeat filtering basic functionality", "[AutoRepeat]")
{
  // RAII helper clears flags before and after this test case runs
  AutoClearAutoRepeatFlags autoClear;

  SECTION("First key down is not an auto-repeat")
  {
    KeyEvent e1 = MakeKeyEvent('A', true);
    REQUIRE(IsAutoRepeat(e1) == false);
  }

  SECTION("Consecutive key downs without key up IS an auto-repeat")
  {
    KeyEvent e1 = MakeKeyEvent('A', true);
    IsAutoRepeat(e1); // Register the first press

    KeyEvent e2 = MakeKeyEvent('A', true);
    REQUIRE(IsAutoRepeat(e2) == true);
  }

  SECTION("Key up resets the state for that key")
  {
    KeyEvent e1 = MakeKeyEvent('A', true);
    IsAutoRepeat(e1);

    KeyEvent e2 = MakeKeyEvent('A', false); // Key Up
    REQUIRE(IsAutoRepeat(e2) == false); // Key up itself shouldn't be auto-repeat

    KeyEvent e3 = MakeKeyEvent('A', true);
    REQUIRE(IsAutoRepeat(e3) == false); // Should NOT be an auto-repeat now
  }
}

TEST_CASE("Handling multiple independent keys", "[AutoRepeat]")
{
  AutoClearAutoRepeatFlags autoClear;

  SECTION("Pressing different keys does not trigger auto-repeat")
  {
    KeyEvent aDown = MakeKeyEvent('A', true);
    KeyEvent bDown = MakeKeyEvent('B', true);

    REQUIRE(IsAutoRepeat(aDown) == false);
    REQUIRE(IsAutoRepeat(bDown) == false);
  }
}

TEST_CASE("Clearing flags manually and via RAII", "[AutoRepeat]")
{
  SECTION("ClearAutoRepeatFlags resets stuck keys")
  {
    AutoClearAutoRepeatFlags autoClear;

    KeyEvent e1 = MakeKeyEvent('X', true);
    IsAutoRepeat(e1);

    // Simulate losing focus or changing game state
    ClearAutoRepeatFlags();

    KeyEvent e2 = MakeKeyEvent('X', true);
    REQUIRE(IsAutoRepeat(e2) == false); // Flag was cleared, so this is a fresh press
  }

  SECTION("RAII struct clears state between separate tests")
  {
    // 1. Create a scope for the first test block
    {
      AutoClearAutoRepeatFlags autoClear;
      KeyEvent e1 = MakeKeyEvent('Z', true);
      IsAutoRepeat(e1);
    } // autoClear destructor runs here, calling ClearAutoRepeatFlags()

    // 2. Create a new scope to verify the state was cleaned up
    {
      AutoClearAutoRepeatFlags autoClear;
      KeyEvent e2 = MakeKeyEvent('Z', true);
      REQUIRE(IsAutoRepeat(e2) == false); // Proves destructor cleaned the state
    }
  }
}

// Testing special keys if your implementation tracks them separately
TEST_CASE("Special keys auto-repeat handling", "[AutoRepeat]")
{
  AutoClearAutoRepeatFlags autoClear;

  SECTION("Special keys register auto-repeat correctly")
  {
    // Using AMJU_KEY_SPACE as a representative value.
    KeyEvent space1 = MakeKeyEvent(KeyType::AMJU_KEY_SPACE, true);
    IsAutoRepeat(space1);

    KeyEvent space2 = MakeKeyEvent(KeyType::AMJU_KEY_SPACE, true);
    REQUIRE(IsAutoRepeat(space2) == true);
  }
}

