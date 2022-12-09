// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <File.h>
#include <StringUtils.h>
#include "Course.h"

namespace Amju
{
Resource* CourseLoader(const std::string& resName)
{
  Course* c = new Course;
  // Remove final extension
  std::string filename = GetFileNoExt(resName);
  if (!c->Load(filename))
  {
    delete c; // TODO res loaders like this should return an RCPtr
    return nullptr;
  }
  return c;
}

bool Course::Load(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    return false;
  }

  int numTopics = 0;
  if (!f.GetInteger(&numTopics))
  {
    f.ReportError("Expected num topics");
    return false;
  }

  for (int i = 0; i < numTopics; i++)
  {
    // TODO Topic factory
    RCPtr<Topic> topic = new Topic;
    if (!topic->Load(&f))
    {
      f.ReportError("Failed to load topic");
      return false;
    }
    AddTopic(topic);
  }

  return true;
}

void Course::AddTopic(Topic* topic)
{
  m_topics.push_back(topic);
}

int Course::GetNumTopics() const
{
  return static_cast<int>(m_topics.size());
}

Topic* Course::GetTopic(const std::string& topicId)
{
  for (RCPtr<Topic> t : m_topics)
  {
    if (t->GetId() == topicId)
    {
      return t;
    }
  }
  return nullptr;
}

Topic* Course::GetTopic(int i)
{
  Assert(i < static_cast<int>(m_topics.size()));
  return m_topics[i];
}

static Course* s_course = nullptr;

void SetCourse(Course* course)
{
  s_course = course;
}

Course* GetCourse()
{
  return s_course;
}
}
