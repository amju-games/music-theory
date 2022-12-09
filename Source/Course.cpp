// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "Course.h"

namespace Amju
{
  bool Course::Load(const std::string& filename)
  {
    return false;
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

}
