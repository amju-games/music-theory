// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "catch.hpp"
#include "CreateDictionary.h"
#include "MusicalTermQuestion.h"
#include "QuestionProgress.h"

using namespace Amju;

TEST_CASE("Get allowable questions", "MusicalTermQuestion")
{
  MusicalTermQuestion mtq;

  Dictionary* dic = CreateDic();
  
  mtq.SetDictionary(dic);

  QuestionProgress qp;
  qp.SetMaxQuestions(4);
  qp.SetQuestionUsed(0);
  qp.SetQuestionUsed(1);

  auto unused = qp.GetUnusedSet();
  REQUIRE(unused == QuestionSet({ 2, 3 }));

// TODO Make question, should use progress so we never duplicate Qs.
  mtq.MakeQuestion(qp);

} 

