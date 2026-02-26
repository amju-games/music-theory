// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <map>
#include "Consts.h"
#include "TimeValue.h"
#include "Utils.h"

static const std::map<std::string, float> TIME_VAL_STRS =
{
  { INPUT_TOKEN_SEMIBREVE,  TIMEVAL_SEMIBREVE },
  { INPUT_TOKEN_MINIM,      TIMEVAL_MINIM },
  { INPUT_TOKEN_CROTCHET,   TIMEVAL_CROTCHET },
  { INPUT_TOKEN_QUAVER,     TIMEVAL_QUAVER },
  { INPUT_TOKEN_SEMIQUAVER, TIMEVAL_SEMIQUAVER },
  { INPUT_TOKEN_QQQ,        TIMEVAL_QQQ },
};

TimeType GetTimeTypeFromToken(const std::string& cs)
{
  auto s(cs);
  Remove(s, 'r'); 
    // This is for rest input format where the duration string
    // also contains 'r'. I think it would be better to deprecate this form
    // and be consitent with pitches, i.e. allow only "<sb> r" or "<r> sb",
    //  not "sbr".

  Remove(s, '*'); 
    // To support hiding the note or rest time value in the
    // final output, intended to support questions like "what is the value of
    // the rest here?"

  static const std::map<std::string, TimeType> TIME_TYPES = 
  {
    { INPUT_TOKEN_SEMIBREVE,         TimeType::SEMIBREVE }, 
    { INPUT_TOKEN_DOTTED_SEMIBREVE,  TimeType::DOTTED_SEMIBREVE  },
    { INPUT_TOKEN_MINIM,             TimeType::MINIM  },
    { INPUT_TOKEN_DOTTED_MINIM,      TimeType::DOTTED_MINIM  },
    { INPUT_TOKEN_CROTCHET,          TimeType::CROTCHET  },
    { INPUT_TOKEN_DOTTED_CROTCHET,   TimeType::DOTTED_CROTCHET  },
    { INPUT_TOKEN_QUAVER,            TimeType::QUAVER  },
    { INPUT_TOKEN_DOTTED_QUAVER,     TimeType::DOTTED_QUAVER  },
    { INPUT_TOKEN_SEMIQUAVER,        TimeType::SEMIQUAVER  },
    { INPUT_TOKEN_DOTTED_SEMIQUAVER, TimeType::DOTTED_SEMIQUAVER  },
    { INPUT_TOKEN_QQQ,               TimeType::QQQ }, 
    { INPUT_TOKEN_DOTTED_QQQ,        TimeType::DOTTED_QQQ  },
  };

  auto it = TIME_TYPES.find(s);
  if (it == TIME_TYPES.end()) return TimeType::ERROR;
  return it->second;
}

float GetTimeVal(const std::string& cs)
{
  auto s(cs);
  Remove(s, 'r'); // rests and notes are treated the same
  Remove(s, '*'); // in case glyph is hidden

  float dot = 1.f;
  if (Contains(s, '.'))
  {
    dot = 1.5f;
    Remove(s, '.');
  }

  auto it = TIME_VAL_STRS.find(s);
  if (it == TIME_VAL_STRS.end())
  {
    return -1;
  }

  return dot * it->second;
}

bool IsImmediateTimeVal(const std::string& cs)
{
  std::string s(cs);
  // Strip dot (.), rest (r), hide (*) qualifiers
  Remove(s, '.');
  Remove(s, 'r');
  Remove(s, '*');
  // TODO Any other qualifiers? NB Use consts!

  // Now we can check the remainder of the string to see if it's a recognised
  //  note type.
  return TIME_VAL_STRS.find(s) != TIME_VAL_STRS.end();
}

bool IsDeferredTimeVal(const std::string& s)
{
  return s.size() > 2 && s[0] == '<' && s.back() == '>' &&
    IsImmediateTimeVal(s.substr(1, s.size() - 2));
}

bool Times::DurationIsSemibreveOrMore() const
{
  return m_timeType >= TimeType::SEMIBREVE;
}

bool Times::IsDotted() const
{
  return (   m_timeType == TimeType::DOTTED_SEMIBREVE 
          || m_timeType == TimeType::DOTTED_MINIM 
          || m_timeType == TimeType::DOTTED_CROTCHET 
          || m_timeType == TimeType::DOTTED_QUAVER 
          || m_timeType == TimeType::DOTTED_SEMIQUAVER
          || m_timeType == TimeType::DOTTED_QQQ
  ); 
}

void Times::SetStartTime(TimeValue startTime)
{
  m_startTime = startTime;
}

void Times::Set(const std::string& timeToken)
{
  m_timeType = GetTimeTypeFromToken(timeToken);
  m_timeValue = GetTimeVal(timeToken);
  m_token = timeToken;
}

void Times::Normalise(float scale)
{
  m_normalisedStartTime = m_startTime * scale;
  m_normalisedDuration = m_timeValue * scale;
}

TimeType Times::GetTimeType() const { return m_timeType; }
TimeValue Times::GetTimeValue() const { return m_timeValue; }
TimeValue Times::GetStartTimeValue() const { return m_startTime; }
float Times::GetNormalisedStartTime() const { return m_normalisedStartTime; } 
float Times::GetNormalisedDuration() const { return m_normalisedDuration; }
std::string Times::GetTimeToken() const { return m_token; }

