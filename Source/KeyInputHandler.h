#pragma once

#include <functional>
#include <map>
#include <EventTypes.h>

namespace Amju
{
// * Key Input Handler *
// Handles keyboard input, e.g. debug keys to show info, reload, etc.
// Maintains a map of key to function.
// Also we register keys for keyboard control with this handler. This
//  prevents clashes between gameplay and debug buttons.
class KeyInputHandler
{
public:
  // The key for our lookup table is a KeyEvent. There is no coord member,
  //  and we want to match all the members.
  using KeyHandlerInfo = KeyEvent; 

  // Handler function for a key event.
  // Return true if the key event is consumed and should not be
  //  processed further.
  // Return false if we don't consume the event (we don't know how
  //  to handle it, etc).
  using KeyHandlerFunction = std::function<bool(const KeyEvent&)>;

  // Pair handler function with text description, to report to console etc.
  using KeyHandlerValue = std::pair<KeyHandlerFunction, std::string>;

public:
  // Handle a key event if there is a function registered for the 
  //  key, up/down flag and possibly other bits.
  // Return true if handled (event consumed).
  bool OnKeyEvent(const KeyEvent&);

  // Add handler function/description pair for the given key event.
  // Allow overwrite of existing handler if `overwrite` is true.
  // Returns true if successful.
  // Returns false if we would overwrite an existing handler and
  //  overwrite is false, and doesn't overwrite.
  // [[nodiscard]] on the result because the point of having this
  //  class is to make sure we aren't remapping keys willy-nilly :)
  [[nodiscard]] bool 
  AddHandler(const KeyHandlerInfo& info, const KeyHandlerValue& value,
    bool overwrite = false);

  // Remove handler func/description for the given event type.
  // Returns true if removed, false if it wasn't there.
  bool RemoveHandler(const KeyHandlerInfo& info);

  // List each handler description on a separate line.
  std::string ListHandlers() const;

private:
  using KeyHandlerMap = std::map<KeyHandlerInfo, KeyHandlerValue>;
  KeyHandlerMap m_map;
};

// This should be Singleton-esque for debug keys, so we can share 
//  across library and game specific states, etc.
// Also, we don't want clashes, so we should just have one handler
//  responsible for all key input.
KeyInputHandler& GetKeyInputHandler(); 

// Convenience functions to create keys 
// Create character KeyEvent
KeyEvent MakeKeyEvent(char key, bool isDown, 
  KeyModifier modifier = KeyModifier::AMJU_KEY_MOD_NONE);

// Create special key event
KeyEvent MakeKeyEvent(KeyType specialKey, bool isDown, 
  KeyModifier modifier = KeyModifier::AMJU_KEY_MOD_NONE);

}

