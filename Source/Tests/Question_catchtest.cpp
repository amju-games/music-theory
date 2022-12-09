// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "catch.hpp"
#include "Dictionary.h"
#include "MultiChoiceQuestion.h"
#include "MusicalTermQuestion.h"

using namespace Amju;

TEST_CASE("Answers can be added and retrieved", "[MultiChoice]")
{
  MultiChoice mc;
  REQUIRE(mc.GetNumAnswers() == 0);

  SECTION("Add some answers")
  {
    const int CORRECT = 0;

    mc.AddAnswer("Red");
    mc.AddAnswer("Green");
    mc.AddAnswer("Blue");
    mc.SetCorrectAnswer(CORRECT);

    REQUIRE(mc.GetNumAnswers() == 3);

    SECTION("Check correct answer")
    {
      REQUIRE(mc.GetCorrectAnswer() == CORRECT);
      REQUIRE(mc.IsAnswerCorrect(CORRECT));
      REQUIRE_FALSE(mc.IsAnswerCorrect(CORRECT + 1));
    }
  }
}

TEST_CASE("Terms can be added and retrieved", "[Dictionary]")
{
  RCPtr<Dictionary> dic(new Dictionary);

  SECTION("Adding terms")
  {
    dic->AddTerm({ "Soft", "Piano" });
    dic->AddTerm({ "Loud", "Forte" });
    dic->AddTerm({ "Sweetly", "Dolce" });
    dic->AddTerm({ "Gracefully", "Grazioso" });

    REQUIRE(dic->GetNumTerms() == 4);

    SECTION("Retrieving terms")
    {
      std::string english, foreign;
      dic->GetTerm(0, &english, &foreign);
      REQUIRE(english == "Soft");
    }

    SECTION("Set up a question")
    {
      MusicalTermQuestion q;
      q.SetDictionary(dic);
      q.MakeQuestion();
      MultiChoice mc = q.GetMultiChoiceAnswers();
    }
  }
}

