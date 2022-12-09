// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

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

Colour GetColour(const char* colourName)
{
  Assert(ROConfig()->Exists(colourName));
  return FromHexString(ROConfig()->GetValue(colourName));
}

}

