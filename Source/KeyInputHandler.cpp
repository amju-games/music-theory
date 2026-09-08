#include <iostream>
#include <sstream>
#include <tuple>
#include "KeyInputHandler.h"

namespace Amju
{
std::ostream& operator<<(std::ostream& os, const KeyEvent& ke)
{
  return os << "Key: " << ke.key << " " << (ke.keyDown ? "down" : "up");
}

bool operator<(const KeyEvent& ke1, const KeyEvent& ke2)
{
  return 
    std::tie(ke1.keyType, ke1.key, ke1.keyDown, ke1.modifier) < 
    std::tie(ke2.keyType, ke2.key, ke2.keyDown, ke2.modifier); 
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

bool KeyInputHandler::RemoveHandler(const KeyHandlerInfo& info)
{
  return m_map.erase(info);
}

bool KeyInputHandler::AddHandler(
  const KeyHandlerInfo& info, const KeyHandlerValue& value,
  bool overwrite)
{
  if (m_map.contains(info) && !overwrite)
  {
#ifdef KEY_INPUT_HANDLER_OVERWRITE_DEBUG
    std::cout << "Overwriting handler for Key Event! The event is: "
      << info 
      << "\n";
#endif
    return false;
  } 
  m_map[info] = value;
  return true;
}

bool KeyInputHandler::OnKeyEvent(const KeyEvent& ke)
{
  auto it = m_map.find(ke);
  if (it == m_map.end()) return false; // no handler registered
  const auto [func, str] = it->second;
  return func(ke);
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

