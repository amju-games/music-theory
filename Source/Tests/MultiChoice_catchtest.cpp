// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "catch.hpp"
#include "MultiChoiceQuestion.h"

using namespace Amju;

TEST_CASE("Test MultiChoice", "MultiChoice")
{
  MultiChoice mc;
  REQUIRE(mc.GetCorrectAnswer() == -1);
  REQUIRE(mc.GetNumAnswers() == 0);

  mc.SetCorrectAnswer(2);
  REQUIRE(mc.GetCorrectAnswer() == 2);

  REQUIRE(mc.IsAnswerCorrect(0) == false);
  REQUIRE(mc.IsAnswerCorrect(1) == false);
  REQUIRE(mc.IsAnswerCorrect(2) == true);

  REQUIRE(mc.IsAnAnswer("aaa") == false);
  mc.AddAnswer("aaa");
  REQUIRE(mc.GetNumAnswers() == 1);
  REQUIRE(mc.IsAnAnswer("aaa") == true);
  mc.AddAnswer("bbb");
  REQUIRE(mc.GetNumAnswers() == 2);
  REQUIRE(mc.IsAnAnswer("bbb") == true);
  mc.AddAnswer("ccc");

  REQUIRE(mc.GetAnswer(0) == "aaa");
  REQUIRE(mc.GetAnswer(1) == "bbb");
  REQUIRE(mc.GetAnswer(2) == "ccc");
}

