#include "Course.h"
#include "CourseManager.h"
#include "GSPagesEdit.h"
#include "UserProfile.h"

namespace Amju
{
void GSPagesEdit::OnActive()
{
  m_livesThisSession = 1;
  GSPages::OnActive();
}

void GSPagesEdit::NextPage()
{
  GSPages::NextPage();

  Course* course = GetCourseManager().GetCourse();
  Assert(course);
  Topic* topic = course->GetTopic(TheUserProfile()->GetCurrentTopic());
  Assert(topic);

  // Display info - TODO show in GUI

  std::cout << "Course: " << course->GetResName() << "\n";
  std::cout << "This topic: " << topic->GetDisplayName() << "\n";
  std::cout << " Current page: " << m_currentPage << "\n";
  std::cout << "This page: " << (m_page ? m_page->GetInfo() : "not set") << "\n";
}

bool GSPagesEdit::OnKeyEvent(const KeyEvent& ke)
{
  // TODO check for some keys, but not 'e' because that takes us here! 
  // Probably also not reloading gui, because that could overwrite changes we are making.

  if (CheckForKey_B_BackToPrevState(ke))
  {
    // TODO Wait, we want to prompt to save, no?
    // Or do we save all changes immediately?
    return true;
  }

  if (ke.keyDown && ke.key == 'n')
  {
    // Next Q
    NextPage();
  }

  return false;
}
}
