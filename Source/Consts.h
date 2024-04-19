// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <Colour.h>

namespace Amju
{
// * Constants *

// Delay before we go to next page (gives time to see blackboard erase)
static const float NEXT_PAGE_TIME = 2.2f;

// Time for which we show a Lurk Msg with auto-hide (i.e. no OK button) - this is
//  to show "Correct" msg etc.
static const float PAGE_LURK_TIME = 1.7f;

// Distance between doors in corridor
static const float DISTANCE_BETWEEN_DOORS = 1.0f;

// User scroll left/right in corridor
static const float MAX_SCROLL_VEL = 300.0f;
static const float SCROLL_DECEL = 280.0f;

// Door rectangle in corridor wait mode
static const float DOOR_RECT_XMIN = -.25f;
static const float DOOR_RECT_XMAX =  .25f;
static const float DOOR_RECT_YMIN = -.6f;
static const float DOOR_RECT_YMAX =  .6f;

// Camera zoom distance when we go into stairs archway to change level
static const float STAIRS_ZOOM_DIST = -100.f;

// Keys for map/config file lookups etc

#define APPNAME  "amjula-mt"

#define KEY_GENERAL "general"
#define KEY_USER_NAME "user_name"

#define DEFAULT_SHARE_TEXT_KEY "default_share_text"
#define DEFAULT_SHARE_TEXT "I'm learning stuff at Amjula Music School! amjula.com #amjula"

#define MIDI_SOUND_FONT "Grand Piano"
// "Jv_drums"

extern const std::string WAV_CORRECT;
extern const std::string WAV_INCORRECT;
extern const std::string WAV_SWIPE_LEFT;
extern const std::string WAV_SWIPE_RIGHT;
extern const std::string WAV_ENTER_STAIRS;
extern const std::string WAV_ENTER_DOOR;
extern const std::string WAV_DOOR_OPEN;
extern const std::string WAV_START_BUTTON;
extern const std::string WAV_CANCEL;
extern const std::string WAV_START_TOPIC;
extern const std::string WAV_BUTTON_CLICK;
extern const std::string WAV_ENTER_TAPPABLE;
extern const std::string WAV_EXIT_TAPPABLE;

#define DEVICE_ID "device_id"
#define DEVICE_OS_VERSION "device_os_version"
#define DEVICE_USER_NAME "device_user_name"
#define CLIENT_VERSION "client_version"
#define SESSION_ID "session_id"

// * Colours *

// GetColour
// Looks up the given colour name in ROConfig
Colour GetColour(const char* colourName);

// Constants to pass into GetColour()
#define COLOUR_TUTORIAL "colour_tutorial"
#define COLOUR_INCORRECT "colour_incorrect"
#define COLOUR_CORRECT "colour_correct"
#define COLOUR_EXPLANATION "colour_explanation"
#define COLOUR_TEXT "colour_text"
#define COLOUR_MUSIC_HIGHLIGHT "colour_highlight"
#define COLOUR_CONFIRM_QUIT "colour_confirm_quit"

}
