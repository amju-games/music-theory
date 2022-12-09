// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "catch.hpp"
#include "Dictionary.h"
#include "MusicalTermQuestion.h"
#include "QuestionProgress.h"

using namespace Amju;

namespace
{
Dictionary* CreateDic()
{
  Dictionary* dic = new Dictionary;

  dic->AddTerm({ "Soft", "Piano" });
  dic->AddTerm({ "Loud", "Forte" });
  dic->AddTerm({ "Sweetly", "Dolce" });
  dic->AddTerm({ "Gracefully", "Grazioso" });

  return dic;
}
}

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
//  mtq.MakeQuestion(qp);

} 

