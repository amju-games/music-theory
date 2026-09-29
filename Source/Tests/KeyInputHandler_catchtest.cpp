#include <iostream>
#include "catch.hpp"
#include <EventTypes.h> // KeyEvent
#include "AutoRepeatFilter.h"		
#include "KeyInputHandler.h"

TEST_CASE("Add/Remove handlers", "[KeyInputHandler]")
{
  using namespace Amju;

  AutoClearAutoRepeatFlags cleaner;

  KeyInputHandler kih;

  // Our handler func modifies this var
  int setByHandlerFunction = 0;

  auto consumed = kih.OnKeyEvent(MakeKeyEvent('a', true));
  REQUIRE(consumed == KeyInputHandler::Result::AMJU_KEY_EVENT_NO_HANDLER_REGISTERED); 

  // Add handler for 'a' key down: value is handler function and 
  //  description pair.
  bool added = kih.AddHandler(
    MakeKeyEvent('a', true), 
    [&](const KeyEvent& ke)->bool 
    { 
      setByHandlerFunction = 666; 
      return true; 
    },
    "Handler for 'a' key!"
  );
  REQUIRE(added == true);

  // Now the same event should be handled and the function executed.
  // BUT WAIT! We now filter out auto-repeat key events! So this next
  //  event gets filtered out!
  consumed = kih.OnKeyEvent(MakeKeyEvent('a', true));
  // Event is rejected as auto-repeat
  REQUIRE(consumed == KeyInputHandler::Result::AMJU_KEY_EVENT_REJECTED_AUTO_REPEAT);
  REQUIRE(setByHandlerFunction == 0); // ..but we DON'T execute func!

  // Key up event to reset auto-repeat flag
  consumed = kih.OnKeyEvent(MakeKeyEvent('a', false));
  // (Event is not mapped to a handler)
  REQUIRE(consumed == KeyInputHandler::Result::AMJU_KEY_EVENT_NO_HANDLER_REGISTERED); 

  // Key down event, not auto repeat due to above key up event
  consumed = kih.OnKeyEvent(MakeKeyEvent('a', true));

  // Handled!
  REQUIRE(consumed == KeyInputHandler::Result::AMJU_KEY_EVENT_CONSUMED); 
  // Handler function changed this value...
  REQUIRE(setByHandlerFunction == 666);

  // Key up event to reset auto-repeat flag
  consumed = kih.OnKeyEvent(MakeKeyEvent('a', false));
  REQUIRE(consumed == KeyInputHandler::Result::AMJU_KEY_EVENT_NO_HANDLER_REGISTERED); 
 
  // Other key events are not handled. 
  consumed = kih.OnKeyEvent(MakeKeyEvent('b', true));
  REQUIRE(consumed == KeyInputHandler::Result::AMJU_KEY_EVENT_NO_HANDLER_REGISTERED); 

  // Remove handler for 'a' key
  bool removed = kih.RemoveHandler(MakeKeyEvent('a', true));
  REQUIRE(removed == true);
  // Now the event is not handled
  consumed = kih.OnKeyEvent(MakeKeyEvent('a', true));
  REQUIRE(consumed == KeyInputHandler::Result::AMJU_KEY_EVENT_NO_HANDLER_REGISTERED); 

  // Try to remove non-existent handler
  removed = kih.RemoveHandler(MakeKeyEvent('b', true));
  REQUIRE(removed == false); // not removed, didn't exist
}

TEST_CASE("List handler functions", "[KeyInputHandler]")
{
  using namespace Amju;

  AutoClearAutoRepeatFlags cleaner;

  KeyInputHandler kih;

  // We don't care about the actual handler function, so every key event
  //  maps to this same do-nothing func.
  KeyInputHandler::KeyHandlerFunction func = 
      [&](const KeyEvent&)->bool { return true; };

  // Add handlers 
  bool added = true;
  added &= kih.AddHandler(MakeKeyEvent('c', true), func, "'c' key");
  added &= kih.AddHandler(MakeKeyEvent('a', true), func, "'a' key down");
  added &= kih.AddHandler(MakeKeyEvent('b', true), func, "'b' key");
  added &= kih.AddHandler(MakeKeyEvent('a', false), func, "'a' key up");
  REQUIRE(added == true);

  // Check ListHandlers
//std::cout << kih.ListHandlers() << "\n";
}

TEST_CASE("Add existing fails unless overwrite flag set", "[KeyInputHandler]")
{
  using namespace Amju;

  AutoClearAutoRepeatFlags cleaner;

  KeyInputHandler kih;

  // We don't care about the actual handler function, so every key event
  //  maps to this same do-nothing func.
  KeyInputHandler::KeyHandlerFunction func = 
      [&](const KeyEvent&)->bool { return true; };

  // Add handlers 
  bool added = true;
  added = kih.AddHandler(MakeKeyEvent('a', true), func, "'a' key down");
  REQUIRE(added == true);

  // Add existing key event -> fails
  added = kih.AddHandler(MakeKeyEvent('a', true), func, "'a' key down");
  REQUIRE(added == false);

  // Add existing key event with overwrite -> ok
  const bool OVERWRITE = true;
  added = kih.AddHandler(MakeKeyEvent('a', true), func, "'a' key down 2",
    OVERWRITE);
  REQUIRE(added == true);

  // Check ListHandlers: name of second handler is listed
//std::cout << kih.ListHandlers() << "\n";
//
}

