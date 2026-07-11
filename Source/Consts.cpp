// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuAssert.h>
#include "Consts.h"
#include "MyROConfig.h"

namespace Amju
{
// OK to update this from version to version, and would be
//  good to define it in a text file. 
const std::string DEFAULT_SHARE_TEXT = "I'm playing Amju Piano Fest! #amjupianofest @amjugames";

const std::string WAV_CORRECT = "bell_tinkle";
const std::string WAV_INCORRECT = "anvil";//"bad1";
const std::string WAV_SWIPE_LEFT = "ok2";
const std::string WAV_SWIPE_RIGHT = "ok2";
const std::string WAV_ENTER_STAIRS = "ok1-transposed";
const std::string WAV_ENTER_DOOR = "ok1";
const std::string WAV_DOOR_OPEN = "doorcreak";
const std::string WAV_START_BUTTON = "good1";
const std::string WAV_CANCEL = "bad1";
const std::string WAV_START_TOPIC = "good1";
const std::string WAV_BUTTON_CLICK = "Sound/wav/click.wav";
const std::string WAV_ENTER_TAPPABLE = "wow1";
const std::string WAV_EXIT_TAPPABLE = "wow2";

Colour GetConstColour(const char* colourName)
{
  Assert(MyROConfig()->Exists(colourName));
  const std::string& colourString = MyROConfig()->GetValue(colourName);
  auto optionalColour = FromHexString(colourString);
  Assert(optionalColour);
  return *optionalColour;
}

}

