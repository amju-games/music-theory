#pragma once

namespace Amju
{
struct KeyEvent;

// Filter auto-repeat key events, where additional key down 
//  events are fired with no intervening key up event.
// Returns true if event is an auto-repeat event.
// TODO Currently ignores modifiers.
bool IsAutoRepeat(const KeyEvent& ke);

// Clear auto-repeat flags. 
// It's possible to get a flag stuck in 'key down' state, e.g.
//  holding down a key then changing app focus so we don't get 
//  the corresponding key up event. 
// Call this in new Game State, and from platform-specific
//  'get focus' callbacks.
void ClearAutoRepeatFlags();

// Use this in tests to clear the auto-repeat flags.
// RAII -- create an instance at the top of each test.
struct AutoClearAutoRepeatFlags
{
  AutoClearAutoRepeatFlags()
  {
    ClearAutoRepeatFlags();
  }

  ~AutoClearAutoRepeatFlags()
  {
    ClearAutoRepeatFlags();
  }
};
}

