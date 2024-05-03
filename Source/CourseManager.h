// * Amjula music theory * (c) Copyright 2024 Juliet Colman

#pragma once

namespace Amju
{
class Course;

// * ICourseManager *
// Get/set/persist the current course, and provide utility functions.
// Map level number, which is an int, to course name, which is a string.
// Players see level numbers, but internally the courses are split into
//  grades, then groups of topics.
class ICourseManager
{
public:
  virtual ~ICourseManager() = default;

  // Load the course for the given level number, a zero-based index into vec of course names.
  // Return true if new level successfully loaded.
  // After we successfully load, we can get the loaded course with GetCurrentCourse().
  virtual bool LoadCourseForLevel(int levelNum) = 0;

  // Get the currently loaded course
  virtual Course* GetCourse() = 0;
};

ICourseManager& GetCourseManager();

class CourseManager : public ICourseManager
{
public:
  CourseManager();
  bool LoadCourseForLevel(int levelNum) override;
  Course* GetCourse() override;
};

// Only required for setting a mock for testing
void SetCourseManager(ICourseManager&);
}
