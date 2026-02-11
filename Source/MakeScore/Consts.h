// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

// Quads have an offset in y, so they line up with glyphs as expected.
// So we have to add this offset to quad y-coords.
static const float QUAD_Y_OFFSET = 0.475f;

// Width of crotchet/minim note head.
// Used to place stems and arrange clashing note heads in chords.
static const float NOTE_HEAD_WIDTH = 0.2f;

// Left and right offset for notes in a chord that would overlap.
// A bit less that a full note head width to make sure there are no
//  unsightly gaps.
static const float NOTE_HEAD_OVERLAP_OFFSET = 0.18f;

// Note positions, accidental positions, in chords etc
// Offset for the accidental for a single note
static const float ACCIDENTAL_X_OFFSET = 0.2f;

// For chords, if we offset further to the left, we use a reduced 
//  offset width to try to limit the space taken up.
// TODO although we might need another offset for double flats.
static const float ACCIDENTAL_EXTRA_OFFSET = 0.15f;

// On a single-line stave with percussion clef, notes go on the middle
//  stave line, which is 4, numbered from 0 (bottom line) and including
//  spaces.
static const int PERCUSSION_STAVE_LINE = 4;

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

// Width of every clef
static const float CLEF_WIDTH = 0.45f;

// Width of key signatures, ignoring the possibility of weird, wide ones
static const float TIME_SIG_WIDTH = 0.3f;

// Barline width for a standard bar line
static const float BAR_LINE_WIDTH = 0.03f;

static const int DEFAULT_PITCH = 69; // default pitch = A above middle C

// Used to position ends of curves, beams and flags
static const float X_OFFSET_RIGHT = 0.25f;

// If no page width is specified
static const float DEFAULT_PAGE_WIDTH = 4.0f;

// If no scale specified
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

