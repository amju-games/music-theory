// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <numeric>
#include "catch.hpp"
#include "CreateDictionary.h"
#include "QuestionPicker.h"

using namespace Amju;

void PickerTest(int maxNumQs)
{
  QuestionProgress prog;
  
  prog.SetMaxQuestions(maxNumQs);

  REQUIRE(prog.GetUsedSet().size() == 0);
  REQUIRE(prog.GetUnusedSet().size() == maxNumQs);
  REQUIRE(prog.GetNumQuestionsUsed() == 0);

  QuestionPicker picker;

  // Pick one unused answer at random.
  // Set the picked answer in prog, so we won't pick it again.
  int correct = picker.PickCorrect(prog); 

  REQUIRE(correct >= 0);
  REQUIRE(correct < maxNumQs);

  QuestionSet used = prog.GetUsedSet();
  REQUIRE(used.size() == 1); 
  REQUIRE(used[0] == correct);

  REQUIRE(prog.GetUnusedSet().size() == maxNumQs - 1);
  REQUIRE(prog.GetNumQuestionsUsed() == 1);
}

TEST_CASE("QuestionPicker pick correct answer", "QuestionPicker")
{
  PickerTest(1);
  PickerTest(2);
  PickerTest(10);
}

TEST_CASE("QuestionPicker populate StringIndexVec", "QuestionPicker")
{
  Dictionary* dic = CreateDicWithDuplicates();
  
  std::vector<int> indices({ 0, 1, 2 });

  int strNum = 0;
  StringIndexVec siVec = QuestionPicker::PopulateStringIndexVecFromDictionary(
    indices,
    *dic,
    strNum);

  int n = indices.size();
  REQUIRE(siVec.size() == n);
  for (int i = 0; i < n; i++)
  {
    REQUIRE(siVec[i].second == indices[i]);
    std::string str[2];
    dic->GetTerm(siVec[i].second, &str[0], &str[1]);
    REQUIRE(str[strNum] == siVec[i].first);
  }
}

TEST_CASE("QuestionPicker get Just Indices", "QuestionPicker")
{
  std::vector<int> inds = { 2, 3, 6, 8, 9, 10 };
  StringIndexVec siVec = {
    { "a", inds[0] },
    { "b", inds[1] },
    { "c", inds[2] },
    { "d", inds[3] },
    { "e", inds[4] },
    { "f", inds[5] },
  };

  REQUIRE(inds.size() == siVec.size());

  std::vector<int> justInds = QuestionPicker::JustIndices(siVec);

  REQUIRE(justInds == inds);
}

TEST_CASE("QuestionPicker remove duplicates in StringIndexVec", "QuestionPicker")
{
  StringIndexVec siVec = {
    { "a", 0 },
    { "a", 1 },
    { "a", 2 }
  };

  StringIndexVec noDups = QuestionPicker::RemoveDuplicatesInStringIndexVec(siVec);

  REQUIRE(noDups.size() == 1);
  REQUIRE(noDups[0].first == "a");
}

TEST_CASE("QuestionPicker remove duplicate answers", "QuestionPicker")
{
  const Dictionary* dic = CreateDicWithDuplicates();

  // Create a vector of indices into the dic, one index per dic entry.
  std::vector<int> indices(dic->GetNumTerms());
  std::iota(indices.begin(), indices.end(), 0); // create indices sequence
  
  REQUIRE(indices.size() == dic->GetNumTerms());

  QuestionPicker picker;
  indices = picker.RemoveDuplicates(indices, *dic);

  REQUIRE(indices.size() < dic->GetNumTerms());
}

TEST_CASE("QuestionPicker get fakes", "QuestionPicker")
{
  const Dictionary* dic = CreateDic();

  int n = dic->GetNumTerms() - 1;
  int correct = 0;
  std::vector<int> indices = QuestionPicker::GetNFakes(n, correct, *dic);

  REQUIRE(indices.size() == n);
  REQUIRE(std::find(indices.begin(), indices.end(), correct) == indices.end());
}
