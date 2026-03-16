// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <algorithm>
#include <numeric> // iota
#include <AmjuAssert.h>
#include <AmjuRand.h>
#include "QuestionPicker.h"

namespace Amju
{
int QuestionPicker::PickCorrect(QuestionProgress& prog) const
{
  QuestionSet unused = prog.GetUnusedSet();
  Assert(!unused.empty());

  // TODO Control randomisation
  int n = unused.size();
  int r = rand() % n;
  int q = unused[r];

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

static void Shuffle(std::vector<int>& vec)
{
  RandomShuffle(vec.begin(), vec.end(), RandomInt); 
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

  // Remove indices which have duplicate answers 
  indices = RemoveDuplicates(indices, dic);

  // Shuffle and return top n
  Shuffle(indices);

  // Copy only, at most, the first n
  std::vector<int> result;
  result.reserve(n);
  int c = 0;
  // TODO Can just use c, no need for i?
  for (int i = 0; i < n; /* Don't imcrement here in case we skip */ )
  {
    if (c == indices.size())
    {
      break; // no more fakes
    }

    // Check the answer for indices[i] is not the same as the correct answer.
    // If it is, skip to the next.
    int r = indices[c];
    dic.GetTerm(r, &question, &ans);
    c++;
    if (ans == correctAns)
    {
      continue;
    }
    // r is ok, not the same as correct answer
    result.push_back(r);
    i++; // here, not in the for(;;)
  }

  return result;
}
}

