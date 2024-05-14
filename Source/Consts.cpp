// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuAssert.h>
#include <ROConfig.h>
#include "Consts.h"

namespace Amju
{
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
  Assert(ROConfig()->Exists(colourName));
  const std::string& colourString = ROConfig()->GetValue(colourName);
  auto optionalColour = FromHexString(colourString);
  Assert(optionalColour);
  return *optionalColour;
}

}

