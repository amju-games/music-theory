// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "catch.hpp"
#include "Course.h"

using namespace Amju;

TEST_CASE("Add topics to a new course", "Course")
{
  Course course;

  Topic* topic1 = new Topic("easy"); // TODO factory, create subclasses, in another test
  Topic* topic2 = new Topic("hard");

  topic1->AddTopicThisUnlocks({ "hard" });

  course.AddTopic(topic1);
  course.AddTopic(topic2);

  REQUIRE(course.GetNumTopics() == 2);

}
