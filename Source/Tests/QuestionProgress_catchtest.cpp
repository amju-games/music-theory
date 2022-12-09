// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "catch.hpp"
#include "QuestionProgress.h"

using namespace Amju;

TEST_CASE("Create QuestionProgress", "QuestionProgress")
{
  QuestionProgress qp;
  qp.SetMaxQuestions(2);
  REQUIRE(qp.GetMaxQuestions() == 2);
}

TEST_CASE("QuestionProgress count questions used", "QuestionProgress")
{
  QuestionProgress qp;
  qp.SetMaxQuestions(2);

  REQUIRE(qp.GetNumQuestionsUsed() == 0);
  qp.SetQuestionUsed(0);
  REQUIRE(qp.GetNumQuestionsUsed() == 1);
  qp.SetQuestionUsed(1);
  REQUIRE(qp.GetNumQuestionsUsed() == 2);

  REQUIRE(qp.AllQuestionsUsed());
}

TEST_CASE("QuestionProgress get used set", "QuestionProgress")
{
  QuestionProgress qp;
  qp.SetMaxQuestions(4);

  REQUIRE(qp.GetUsedSet().size() == 0); 

  qp.SetQuestionUsed(0);
  REQUIRE(qp.GetUsedSet() == QuestionSet({ 0 })); 
  REQUIRE(qp.GetUnusedSet() == QuestionSet({ 1, 2, 3 })); 

  qp.SetQuestionUsed(2);
  REQUIRE(qp.GetUsedSet() == QuestionSet({ 0, 2 })); 
  REQUIRE(qp.GetUnusedSet() == QuestionSet({ 1, 3 })); 

  qp.SetQuestionUsed(1);
  REQUIRE(qp.GetUsedSet() == QuestionSet({ 0, 1, 2 })); 
  REQUIRE(qp.GetUnusedSet() == QuestionSet({ 3 })); 

  qp.SetQuestionUsed(3);
  REQUIRE(qp.GetUsedSet() == QuestionSet({ 0, 1, 2, 3 })); 
  REQUIRE(qp.GetUnusedSet() == QuestionSet({ })); 
}

