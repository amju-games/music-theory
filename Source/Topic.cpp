// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "Topic.h"

namespace Amju
{
  Topic::Topic(const std::string& id)
  {
    SetId(id);
  }

  const std::string& Topic::GetId() const
  {
    return m_id;
  }

  void Topic::SetId(const std::string& id)
  {
    m_id = id;
  }

  const std::string& Topic::GetDisplayName() const
  {
    return m_displayName;
  }

  void Topic::SetDisplayName(const std::string& displayName)
  {
    m_displayName = displayName;
  }

  void Topic::AddTopicThisUnlocks(const std::string& unlocks)
  {
  }

  void Topic::OnCompleted()
  {
  }
}

