#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>
#include "AutoRepeatFilter.h"
#include "KeyInputHandler.h"

namespace Amju
{
static KeyEvent Sanitise(const KeyEvent& ke)
{
  // Set char to lower case, and zero if this is a special key event.
  auto copy(ke);
  copy.key = std::tolower(copy.key);
  if (copy.keyType != AMJU_KEY_CHAR) copy.key = 0;
  return copy;
}

KeyInputHandler& GetKeyInputHandler()
{
  static KeyInputHandler kih;
  return kih;
}

static const std::string KeyTypeStr(KeyType kt)
{
  static const std::vector<std::string> STRS = 
  {
    "char", // AMJU_KEY_CHAR,  // printable character
    "up  ", // AMJU_KEY_UP,    // up arrow
    "down", // AMJU_KEY_DOWN,
    "left", // AMJU_KEY_LEFT,
    "right", // AMJU_KEY_RIGHT,
    "enter", // AMJU_KEY_ENTER,
    "space", // AMJU_KEY_SPACE,
    "esc ", // AMJU_KEY_ESC,
    "bksp", // AMJU_KEY_BACKSPACE,
    "del ", // AMJU_KEY_DELETE,
    "pgup", // AMJU_KEY_PAGE_UP,
    "pgdn", // AMJU_KEY_PAGE_DOWN,
    "home", // AMJU_KEY_HOME,
    "end ", // AMJU_KEY_END,
    "alt ", // AMJU_KEY_ALT,
  };
  return STRS[kt];
}

std::ostream& operator<<(std::ostream& os, const KeyEvent& ke)
{
  return os << "Key: " 
    << (ke.keyType == AMJU_KEY_CHAR ? 
        std::string(1, ke.key) : KeyTypeStr(ke.keyType))
    << " " << (ke.keyDown ? "down" : "up");
}

bool operator<(const KeyEvent& ke1, const KeyEvent& ke2)
{
  return 
    std::tie(ke1.keyType, ke1.key, ke1.keyDown, ke1.modifier) < 
    std::tie(ke2.keyType, ke2.key, ke2.keyDown, ke2.modifier); 
}

void KeyInputHandler::Clear()
{
  m_map.clear();
}

std::string KeyInputHandler::ListHandlers() const
{
  std::stringstream ss;
  for (const auto& [key, val] : m_map)
  {
    const auto& [handler, description] = val;
    ss << key << "\t" << description << "\n";  
  }
  return ss.str();
}

bool KeyInputHandler::RemoveHandler(const KeyEvent& ke)
{
  return m_map.erase(Sanitise(ke));
}

bool KeyInputHandler::AddHandler(
  const KeyEvent& ke, 
  const KeyHandlerFunction func,
  const std::string& description,
  bool overwrite)
{
  // Set char to lower case, and zero if this is a special key event.
  auto copy = Sanitise(ke);

  if (m_map.contains(copy) && !overwrite)
  {
#ifdef KEY_INPUT_HANDLER_OVERWRITE_DEBUG
    std::cout << "Overwriting handler for Key Event! The event is: "
      << info 
      << "\n";
#endif
    return false;
  } 
  m_map[copy] = { func, description };
  return true;
}

KeyInputHandler::Result KeyInputHandler::OnKeyEvent(const KeyEvent& ke)
{
  if (IsAutoRepeat(ke))
  {
    // Ignore event but return true to say 'consumed'.
    return Result::AMJU_KEY_EVENT_REJECTED_AUTO_REPEAT;
  }

  // Set char to lower case, and zero if this is a special key event.
  auto copy = Sanitise(ke);

  auto it = m_map.find(copy);
  if (it == m_map.end()) 
  {
    return Result::AMJU_KEY_EVENT_NO_HANDLER_REGISTERED; 
  }
  const auto [func, str] = it->second;
  // Call the handler function: it returns true if handled.
  const bool ret = func(copy);
  return ret ? Result::AMJU_KEY_EVENT_CONSUMED : 
               Result::AMJU_KEY_EVENT_NOT_CONSUMED;
}

KeyEvent MakeKeyEvent(char key, bool isDown, 
  KeyModifier modifier)
{
  KeyEvent ke;
  ke.keyType = AMJU_KEY_CHAR;
  ke.key = key;
  ke.keyDown = isDown;
  ke.modifier = static_cast<unsigned int>(modifier);
  return ke;
}

KeyEvent MakeKeyEvent(KeyType specialKey, bool isDown, 
  KeyModifier modifier)
{
  KeyEvent ke;
  ke.keyType = specialKey;
  ke.key = 0;
  ke.keyDown = isDown;
  ke.modifier = static_cast<unsigned int>(modifier);
  return ke;
}
}

