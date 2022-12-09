// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <algorithm>
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
  for (int i = 0; i < 2; i++)
  {
    StringIndexVec siVec = PopulateStringIndexVecFromDictionary(result, dic, i);
    siVec = RemoveDuplicatesInStringIndexVec(siVec);
    result = JustIndices(siVec);
  }

  return result;
}

void Shuffle(std::vector<int>&)
{
}

std::vector<int> QuestionPicker::GetNFakes(
  int n, int correct, const Dictionary& dic) 
{
  // Get the correct answer, to make sure we don't include any fakes with the
  //  same answer.
  std::string question, ans, correctAns;
  dic.GetTerm(correct, &question, &correctAns);

  // Create vec of indices into dictionary, but remove the correct index.
  std::vector<int> indices(dic.GetNumTerms());
  std::iota(indices.begin(), indices.end(), 0);
  indices.erase(std::remove(indices.begin(), indices.end(), correct), indices.end());

  // Remove indices which have duplicate answers (or questions, if we
  //  flip Q/A).
  // TODO MAKE SURE NONE OF THE ANSWERS ARE THE SAME AS THE ANSWER FOR THE 'CORRECT' INDEX!!!
  indices = RemoveDuplicates(indices, dic);

  // Shuffle and return top n
  Shuffle(indices);

  // Copy only, at most, the first n
  std::vector<int> result;
  result.reserve(n);
  int c = 0;
  for (int i = 0; i < n; i++)
  {
    // Check the answer for indices[i] is not the same as the correct answer.
    // If it is, skip to the next.
    if (c == indices.size())
    {
      break; // no more fakes
    }
    int r = indices[c];
    dic.GetTerm(r, &question, &ans);
    c++;
    if (ans == correctAns)
    {
      continue;
    }
    // r is ok, not the same as correct answer
    result.push_back(r);
  }

  // Previously: does not check for inclusion of a fake with the correct answer.
  // Copy at most n -- not more than the number of indices!
  //std::copy(indices.begin(), indices.begin() + std::min(n, static_cast<int>(indices.size())), std::back_inserter(result));

  return result;
}
}

