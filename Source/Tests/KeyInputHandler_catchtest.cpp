#include <iostream>
#include "catch.hpp"
#include <EventTypes.h> // KeyEvent
#include "KeyInputHandler.h"

TEST_CASE("Add/Remove handlers", "[KeyInputHandler]")
{
  using namespace Amju;

  KeyInputHandler kih;

  // Our handler func modifies this var
  int setByHandlerFunction = 0;

  bool consumed = kih.OnKeyEvent(MakeKeyEvent('a', true));
  REQUIRE(consumed == false); // no handler 

  // Add handler for 'a' key down: value is handler function and 
  //  description pair.
  bool added = kih.AddHandler(
    MakeKeyEvent('a', true), 
    { 
      [&](const KeyEvent& ke)->bool 
      { 
        setByHandlerFunction = 666; 
        return true; 
      },
      "Handler for 'a' key!"
    }
  );
  REQUIRE(added == true);

  // Now the same event should be handled and the function executed.
  consumed = kih.OnKeyEvent(MakeKeyEvent('a', true));

  REQUIRE(consumed == true); // handled!
  REQUIRE(setByHandlerFunction == 666);
 
  // Other key events are not handled. 
  consumed = kih.OnKeyEvent(MakeKeyEvent('b', true));
  REQUIRE(consumed == false); // not handled!

  // Remove handler for 'a' key
  bool removed = kih.RemoveHandler(MakeKeyEvent('a', true));
  REQUIRE(removed == true);
  // Now the event is not handled
  consumed = kih.OnKeyEvent(MakeKeyEvent('a', true));
  REQUIRE(consumed == false);

  // Try to remove non-existent handler
  removed = kih.RemoveHandler(MakeKeyEvent('b', true));
  REQUIRE(removed == false); // not removed, didn't exist
}

TEST_CASE("List handler functions", "[KeyInputHandler]")
{
  using namespace Amju;

  KeyInputHandler kih;

  // We don't care about the actual handler function, so every key event
  //  maps to this same do-nothing func.
  KeyInputHandler::KeyHandlerFunction func = 
      [&](const KeyEvent&)->bool { return true; };

  // Add handlers 
  bool added = true;
  added &= kih.AddHandler(MakeKeyEvent('c', true), { func, "'c' key" });
  added &= kih.AddHandler(MakeKeyEvent('a', true), { func, "'a' key down" });
  added &= kih.AddHandler(MakeKeyEvent('b', true), { func, "'b' key" });
  added &= kih.AddHandler(MakeKeyEvent('a', false), { func, "'a' key up" });
  REQUIRE(added == true);

  // Check ListHandlers
//std::cout << kih.ListHandlers() << "\n";
}

TEST_CASE("Add existing fails unless overwrite flag set", "[KeyInputHandler]")
{
  using namespace Amju;

  KeyInputHandler kih;

  // We don't care about the actual handler function, so every key event
  //  maps to this same do-nothing func.
  KeyInputHandler::KeyHandlerFunction func = 
      [&](const KeyEvent&)->bool { return true; };

  // Add handlers 
  bool added = true;
  added = kih.AddHandler(MakeKeyEvent('a', true), { func, "'a' key down" });
  REQUIRE(added == true);

  // Add existing key event -> fails
  added = kih.AddHandler(MakeKeyEvent('a', true), { func, "'a' key down" });
  REQUIRE(added == false);

  // Add existing key event with overwrite -> ok
  const bool OVERWRITE = true;
  added = kih.AddHandler(MakeKeyEvent('a', true), { func, "'a' key down 2" },
    OVERWRITE);
  REQUIRE(added == true);

  // Check ListHandlers: name of second handler is listed
//std::cout << kih.ListHandlers() << "\n";
}

