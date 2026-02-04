// * Amjula Music Theory *
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.
// (c) Copyright Juliet Colman 2026

#pragma once

#include <string>

// * Times *
// Unifies the different time concepts in use:
// TimeType: enum: one of the possible time values for a note/rest.
// TimeValue: duration of a note/rest, in units of crotchets.
// NormalisedStartTime: normalised time at which note/rest is played in the 
//  piece. Normalised meaning between 0 and 1.
// NormalisedDuration: time value scaled down to a fraction of the duration
//  of the entire piece. Between 0 and 1.
// TimeToken: the string used to describe the duration of the note or rest,
//  at the parsing stage. Used for testing and comments.
class Times;

// TimeType: enum: one of the possible time values for a note/rest.
enum class TimeType
{
  NOT_SET,
  ERROR,

  SEMIQUAVER,
  DOTTED_SEMIQUAVER,
  QUAVER,
  DOTTED_QUAVER,
  CROTCHET,
  DOTTED_CROTCHET,
  MINIM,
  DOTTED_MINIM,
  SEMIBREVE,
  DOTTED_SEMIBREVE,
};

// TimeValue: duration of a note/rest, in units of crotchets.
using TimeValue = float;
static const float TIMEVAL_SEMIBREVE = 4.f;
static const float TIMEVAL_MINIM = TIMEVAL_SEMIBREVE / 2.f;
static const float TIMEVAL_CROTCHET = TIMEVAL_MINIM / 2.f;
static const float TIMEVAL_QUAVER = TIMEVAL_CROTCHET / 2.f;
static const float TIMEVAL_SEMIQUAVER = TIMEVAL_QUAVER / 2.f;

// * Times *
class Times
{
public:
  Times() = default;

  // Probably only used for testing.
  explicit Times(const std::string& token) { Set(token); }

  // Set from input token at parsing stage.
  void Set(const std::string& timeToken);

  // Set start time of note/rest event, in units of TimeValue, i.e.
  //  number of crotchets since start of piece.
  void SetStartTime(TimeValue startTime);

  // Call to scale start time and duration 
  // (Purpose is to normalise times so the duration of the entire
  //  piece is 1.)
  void Normalise(float scale);

  // Explicit test for semibreve token, useful because the width is
  //  different to other notes.
  bool DurationIsSemibreveOrMore() const;

  // Test for dotted type
  bool IsDotted() const;

  // TimeType: enum: one of the possible time values for a note/rest.
  TimeType GetTimeType() const;

  // TimeValue: duration of a note/rest, in units of crotchets.
  TimeValue GetTimeValue() const;

  // Get start time, as a TimeValue, i.e. a float in crotchet units, from
  //  the start of the piece.
  TimeValue GetStartTimeValue() const;

  // NormalisedStartTime: normalised time at which note/rest is played in the 
  //  piece, between 0 and 1.
  float GetNormalisedStartTime() const;

  // NormalisedDuration: time value scaled down to a fraction of the duration
  //  of the entire piece. Between 0 and 1.
  float GetNormalisedDuration() const;

  // TimeToken: the string used to describe the duration of the note or rest,
  //  at the parsing stage. Used for testing and comments.
  std::string GetTimeToken() const;

private:
  TimeType m_timeType = TimeType::NOT_SET;
  TimeValue m_timeValue = 0;
  TimeValue m_startTime = 0; 
  float m_normalisedStartTime = 0; // scaled version of m_startTime
  float m_normalisedDuration = 0; // scaled version of m_timeValue
  std::string m_token;
};

// Finds the time value for the given string. E.g. "sb" (semibreve) 
//  will give a result of TIMEVAL_SEMIBREVE. 
// This is a numeric value, in units of crotchets.
// Dots are respected, so e.g. "sb." will give a result of
//  1.5 * TIMEVAL_SEMIBREVE.
// Double-dots are not supported.
float GetTimeVal(const std::string& s);

// Finds the time type for the given string. E.g. "sb" (semibreve) 
//  will give a result of TimeType::SEMIBREVE. 
// This is an enum.
// Dots are respected, so e.g. "sb." will give a result of
//  TimeType::DOTTED_SEMIBREVE.
// Double-dots are not supported.
TimeType GetTimeTypeFromToken(const std::string& s);

// Return true if this token specifies a time value, which should immediately
//  generate a note.
bool IsImmediateTimeVal(const std::string& s);

// Return true if this token specifies a time value, which sets the time
//  for subsequent pitch tokens.
bool IsDeferredTimeVal(const std::string& s);

