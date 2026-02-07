// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

// Width of crotchet/minim note head.
// Used to place stems and arrange clashing note heads in chords.
static const float NOTE_HEAD_WIDTH = 0.2f;

// Note positions, accidental positions, in chords etc
// Offset for the accidental for a single note
static const float ACCIDENTAL_X_OFFSET = 0.2f;

// For chords, if we offset further to the left, we use a reduced 
//  offset width to try to limit the space taken up.
// TODO although we might need another offset for double flats.
static const float ACCIDENTAL_EXTRA_OFFSET = 0.15f;

// Don't start TIMEs etc at zero, to avoid notes etc being highlighted
//  before animation starts. So min time is this value, not zero.
static const float MIN_START_TIME = 0.0001f;

// In our coordinate system, with a scale of 1, this is the 
//  distance between stave lines
static const float STAVE_LINE_GAP = 0.1f;

// X-offsets for up and down stems
static const float STEM_UP_X_OFFSET = 0.24f;
static const float STEM_DOWN_X_OFFSET = 0.07f;

// Stem width
static const float STEM_W  = 0.022f;

// Standard stem height for a single note
static const float DEFAULT_STEM_H  = 3.25f; // in units of stave line gaps

static const int DEFAULT_PITCH = 69; // default pitch = A above middle C

static const float X_OFFSET_RIGHT = 0.25f;

static const float DEFAULT_PAGE_WIDTH = 4.0f;

// Height of stave line and glyphs on it, relative to bottom of bar lines.
static const float DEFAULT_HEIGHT = 0.2f;

static const float DEFAULT_SCALE = 1.0f;

// For double stave layout, this is the distance between the top stave line in
//  the first and second staves (in fact, between any two corresponding
//  stave lines, of course).
static const float DOUBLE_STAVE_DISTANCE = -0.9f;

// This is the full list of time types supported in the input.
extern const char* INPUT_TOKEN_SEMIQUAVER;
extern const char* INPUT_TOKEN_DOTTED_SEMIQUAVER;
extern const char* INPUT_TOKEN_QUAVER;
extern const char* INPUT_TOKEN_DOTTED_QUAVER;
extern const char* INPUT_TOKEN_CROTCHET;
extern const char* INPUT_TOKEN_DOTTED_CROTCHET;
extern const char* INPUT_TOKEN_MINIM;
extern const char* INPUT_TOKEN_DOTTED_MINIM;
extern const char* INPUT_TOKEN_SEMIBREVE;
extern const char* INPUT_TOKEN_DOTTED_SEMIBREVE;

static const char TEXT_QUOTE_OPEN = '[';
static const char TEXT_QUOTE_CLOSE = ']';

