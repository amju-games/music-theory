// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <RCPtr.h>

namespace Amju
{
  // * UserProfile *
  // This represents the student's progress through a Course, and other non-course
  //  info, e.g. avatar, student name, etc.
  class UserProfile
  {
  public:
    bool Load(const std::string& userName);

  private:
  };
}
