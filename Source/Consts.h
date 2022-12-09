// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

namespace Amju
{
// * Constants *

// Distance between doors in corridor
static const float DISTANCE_BETWEEN_DOORS = 135;

// Door opening/camera zoom  animation
static const float DOOR_OPEN_ROT_VEL = 1.0f; // rad/sec
static const float MAX_DOOR_ANGLE = 2.0f;
static const float CAMERA_START = 100.0f;
static const float CAM_ZOOM_MULT = 20.0f;

// User scroll left/right in corridor
static const float MAX_SCROLL_VEL = 300.0f;
static const float SCROLL_DECEL = 280.0f;

// Door rectangle in corridor wait mode
static const float DOOR_RECT_XMIN = -.25f;
static const float DOOR_RECT_XMAX =  .25f;
static const float DOOR_RECT_YMIN = -.6f;
static const float DOOR_RECT_YMAX =  .6f;

// Keys for map/config file lookups etc

#define APPNAME  "amjula-mt"

#define KEY_GENERAL "general"
#define KEY_USER_NAME "user_name"
#define KEY_TOPICS "topics"
#define KEY_TOPIC_UNLOCKED "topic_unlocked_"

#define DEFAULT_SHARE_TEXT_KEY "default_share_text"
#define DEFAULT_SHARE_TEXT "I'm learning stuff at Amjula Music School! amjula.com #amjula"

#define MIDI_SOUND_FONT "Grand Piano"

#define WAV_CORRECT "good1"
#define WAV_INCORRECT "bad1"
#define WAV_SWIPE_LEFT "ok2"
#define WAV_SWIPE_RIGHT "ok2"
#define WAV_ENTER_DOOR "ok1"
#define WAV_DOOR_OPEN "doorcreak"
#define WAV_START_BUTTON "good1"
#define WAV_CANCEL "bad1"
#define WAV_START_TOPIC "good1"
#define WAV_BUTTON_CLICK "Sound/wav/click.wav"
#define WAV_ENTER_TAPPABLE "wow1"
#define WAV_EXIT_TAPPABLE "wow2"

}
