#pragma once

namespace Amju
{
struct KeyEvent;

// Filter auto-repeat key events, where additional key down 
//  events are fired with no intervening key up event.
// Returns true if event is an auto-repeat event.
// TODO Currently ignores modifiers and special keys.
bool IsAutoRepeat(const KeyEvent& ke);
}

