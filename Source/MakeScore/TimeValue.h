// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>

// Time values
using TimeValue = float;
static const float TIMEVAL_SEMIBREVE = 1.f;
static const float TIMEVAL_MINIM = TIMEVAL_SEMIBREVE / 2.f;
static const float TIMEVAL_CROTCHET = TIMEVAL_MINIM / 2.f;
static const float TIMEVAL_QUAVER = TIMEVAL_CROTCHET / 2.f;
static const float TIMEVAL_SEMIQUAVER = TIMEVAL_QUAVER / 2.f;

// Finds the time value for the given string. E.g. "sb" (semibreve) 
//  will give a result of TIMEVAL_SEMIBREVE. 
// Dots are respected, so e.g. "sb." will give a result of
//  1.5 * TIMEVAL_SEMIBREVE.
float GetTimeVal(std::string s);

// Return true if this token specifies a time value, which should immediately
//  generate a note.
bool IsImmediateTimeVal(const std::string& s);

// Return true if this token spcifies a time value, which sets the time
//  for subsequent pitch tokens.
bool IsDeferredTimeVal(const std::string& s);
