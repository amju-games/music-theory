#include <ResourceManager.h>
#include "CourseManager.h"

namespace Amju
{
ICourseManager& GetCourseManager()
{
  static CourseManager cm;
  return cm;
}

static Course* s_course = nullptr;

void SetCourse(Course* course)
{
  s_course = course;
}

CourseManager::CourseManager()
{
}

bool CourseManager::LoadCourseForLevel(int levelNum)
{
  // Load the course which this app presents to the user: we only expect there to
  //  be one instance. We could potentially load this depending on config/user choice.
  std::string courseFile = "Course/grade-1-level-" + std::to_string(levelNum) + ".txt.course";
  Course* course = (Course*)TheResourceManager::Instance()->GetRes(courseFile);
  SetCourse(course);
  return course != nullptr;
}

Course* CourseManager::GetCourse()
{
  return s_course;
}
}