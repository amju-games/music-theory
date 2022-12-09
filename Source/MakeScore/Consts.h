// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

static const int MAX_NUM_STAVES = 4; // for SATB

static const int DEFAULT_PITCH = 69; // default pitch = A above middle C

static const float X_OFFSET_RIGHT = 0.25f;

static const float DEFAULT_PAGE_WIDTH = 4.0f;

// Height of stave line and glyphs on it, relative to bottom of bar lines.
static const float DEFAULT_HEIGHT = 0.2f;

static const float DEFAULT_SCALE = 0.6f;

// For double stave layout, this is the distance between the top stave line in
//  the first and second staves (in fact, between any two corresponding
//  stave lines, of course).
static const float DOUBLE_STAVE_DISTANCE = -0.9f;

extern const char* INPUT_TOKEN_SEMIQUAVER;
extern const char* INPUT_TOKEN_QUAVER;
extern const char* INPUT_TOKEN_CROTCHET;
extern const char* INPUT_TOKEN_MINIM;
extern const char* INPUT_TOKEN_SEMIBREVE;

static const char TEXT_QUOTE_OPEN = '[';
static const char TEXT_QUOTE_CLOSE = ']';

