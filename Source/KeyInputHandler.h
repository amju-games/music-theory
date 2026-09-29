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
  // Handler function for a key event.
  // Return true if the key event is consumed and should not be
  //  processed further.
  // Return false if we don't consume the event (we don't know how
  //  to handle it, etc).
  using KeyHandlerFunction = std::function<bool(const KeyEvent&)>;

  // Pair handler function with text description, to report to console etc.
  using KeyHandlerValue = std::pair<KeyHandlerFunction, std::string>;

public:
  enum class Result
  {
    AMJU_KEY_EVENT_CONSUMED,
    AMJU_KEY_EVENT_NOT_CONSUMED,
    AMJU_KEY_EVENT_NO_HANDLER_REGISTERED,
    AMJU_KEY_EVENT_REJECTED_AUTO_REPEAT,
  };

  // Handle a key event if there is a function registered for the 
  //  key, up/down flag and possibly modifier.
  // Return one of the above results.
  // NB All chars are converted to lower case when adding to map
  //  and in OnKeyEvent.
  Result OnKeyEvent(const KeyEvent&);

  // Add handler function/description pair for the given key event.
  // Allow overwrite of existing handler if `overwrite` is true.
  // Returns true if successful.
  // Returns false if we would overwrite an existing handler and
  //  overwrite is false -- and doesn't overwrite the prev handler.
  // [[nodiscard]] on the result because the point of having this
  //  class is to make sure we aren't remapping keys willy-nilly :)
  // NB All chars are converted to lower case when adding to map
  //  and in OnKeyEvent.
  [[nodiscard]] bool 
  AddHandler(
    const KeyEvent& info, 
    KeyHandlerFunction func,
    const std::string& description,
    bool overwrite = false);

  // Remove handler func/description for the given event type.
  // Returns true if removed, false if it wasn't there.
  bool RemoveHandler(const KeyEvent& info);

  // List each handler description on a separate line.
  std::string ListHandlers() const;

  // Trash all handlers
  void Clear();

private:
  using KeyHandlerMap = std::map<KeyEvent, KeyHandlerValue>;
  KeyHandlerMap m_map;
};

// This should be Singleton-esque for debug keys, so we can share 
//  across library and game specific states, etc.
// Also, we don't want clashes, so we should just have one handler
//  responsible for all key input.
KeyInputHandler& GetKeyInputHandler(); 

// Convenience functions to create keys 
// Create character KeyEvent
KeyEvent MakeKeyEvent(char key, bool isDown = true, 
  KeyModifier modifier = KeyModifier::AMJU_KEY_MOD_NONE);

// Create special key event
KeyEvent MakeKeyEvent(KeyType specialKey, bool isDown = true, 
  KeyModifier modifier = KeyModifier::AMJU_KEY_MOD_NONE);

// Comparison op for map
bool operator<(const KeyEvent& ke1, const KeyEvent& ke2);
}

