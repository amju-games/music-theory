// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <numeric> // iota
#include <AmjuAssert.h>

#include "QuestionPicker.h"

namespace Amju
{
int QuestionPicker::PickCorrect(QuestionProgress& prog) const
{
  QuestionSet unused = prog.GetUnusedSet();
  Assert(!unused.empty());
  int q = unused[0]; // TODO just pick first unused for now
  prog.SetQuestionUsed(q);
  return q;
}

StringIndexVec QuestionPicker::RemoveDuplicatesInStringIndexVec(
  const StringIndexVec& csiVec)
{
  StringIndexVec siVec(csiVec);

  auto lessthan = [](const StringIndex& si1, const StringIndex& si2)
  {
    return si1.first < si2.first;
  };

  auto equal = [](const StringIndex& si1, const StringIndex& si2)
  {
    return si1.first == si2.first;
  };

  // Sort string/index pairs by string
  std::sort(siVec.begin(), siVec.end(), lessthan);

  // Now we can remove duplicates with std::unique/erase
  siVec.erase(std::unique(siVec.begin(), siVec.end(), equal), siVec.end());

  return siVec;
}

// Populate a vector of (string, int) pairs.
// The string is one of the strings in a dictionary entry.
// The int is the index of that dictionary entry in the given dictionary.
StringIndexVec QuestionPicker::PopulateStringIndexVecFromDictionary(
  const std::vector<int>& indices,
  const Dictionary& dic, 
  int stringNum)
{
  StringIndexVec result;
  for (int index : indices)
  {
    std::string str[2];
    dic.GetTerm(index, &str[0], &str[1]);
    result.push_back(StringIndex(str[stringNum], index));
  }
  return result; 
}

std::vector<int> QuestionPicker::JustIndices(const StringIndexVec& siVec)
{
  std::vector<int> result;
  result.reserve(siVec.size());
  for (const StringIndex& si : siVec)
  {
    result.push_back(si.second);
  }
  return result;
}

std::vector<int> QuestionPicker::RemoveDuplicates(
  const std::vector<int>& indices,
  const Dictionary& dic) 
{
  std::vector<int> result(indices);

  // Check the indices for Dictionary entries which have the same Q or A.
  StringIndexVec siVec = PopulateStringIndexVecFromDictionary(result, dic, 0);
  siVec = RemoveDuplicatesInStringIndexVec(siVec); 
  result = JustIndices(siVec);

  return result;
}

std::vector<int> QuestionPicker::GetNFakes(
  int n, int correct, const Dictionary& dic) 
{
  // Create vec of indices into dictionary, but remove the correct index.
  std::vector<int> result(dic.GetNumTerms());
  std::iota(result.begin(), result.end(), 0);

  // Remove indices which have duplicate answers (or questions, if we
  //  flip Q/A).

  // Shuffle and return top n

  return result;
}
}

