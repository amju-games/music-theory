// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "QuestionProgress.h"
#include "Dictionary.h"

namespace Amju
{
using StringIndex = std::pair<std::string, int>;
using StringIndexVec = std::vector<StringIndex>;

class QuestionPicker
{
public:
  // Pick one unused question, returning an int index into a Dictionary.
  // Set the picked question as used in prog, so we won't pick it again.
  int PickCorrect(QuestionProgress& prog) const;

  // Helper functions
  static std::vector<int> RemoveDuplicates(
    const std::vector<int>& vec,
    const Dictionary& dic);

  static std::vector<int> GetNFakes(
    int n, int correct, const Dictionary& dic);

  // Populate a vector of (string, int) pairs.
  // The string is one of the strings in a dictionary entry.
  // The int is the index of that dictionary entry in the given dictionary.
  static StringIndexVec PopulateStringIndexVecFromDictionary(
    const std::vector<int>& indices,
    const Dictionary& dic,
    int stringNum);

  // Remove elements from StringIndexVec, which duplicate an existing string.
  static StringIndexVec RemoveDuplicatesInStringIndexVec(
    const StringIndexVec& csiVec);

  // Copy just the indices from siVec into a vector of ints.
  static std::vector<int> JustIndices(const StringIndexVec& siVec);
}; 
}

