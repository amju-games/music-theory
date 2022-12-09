// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>

// Return true if token defines a pitch for use with subsequent tokens
bool IsDeferredPitch(const std::string& s);

// Return true if token defines a pitch, and should generate a note
//  immediately
bool IsImmediatePitch(const std::string& s);

// Return MIDI pitch for the given token, which should be a deferred
//  or immediate pitch value.
int GetPitch(const std::string& s);
