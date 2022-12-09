// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <vector>
#include <RCPtr.h>
#include "Topic.h"

namespace Amju
{
  // * Course *
  // A course is an ordered collection of Topics, which are units of learning material.
  class Course : public RefCounted
  {
  public:
    bool Load(const std::string& filename);

    // Add topic to the end of the sequence
    void AddTopic(Topic* topic);

    int GetNumTopics() const;
    Topic* GetTopic(const std::string& topicId);
    Topic* GetTopic(int i);

  private:
    // Topics are displayed to the user in this order. The ordering should make sense
    //  from a learning point of view, i.e. later topics build on earlier ones.
    std::vector<RCPtr<Topic>> m_topics;
  };
}

