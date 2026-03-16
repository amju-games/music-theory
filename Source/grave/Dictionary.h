// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <string>
#include <vector>
#include <ResourceManager.h>
#include <StringUtils.h>

namespace Amju
{
// * Dictionary *
// Dictionary for musical terms etc.
// Contents can be loaded from file.
// We can also represent music as strings, so the dictionary can be used to store
//  intervals and their names, or a score and some remarks about it, etc.
class Dictionary : public Resource
{
public:
  bool Load(const std::string& filename);
  int GetNumTerms() const;

  // Get two associated strings with optional third string. 
  // In current use case, first string is "question", second string is 
  //  "answer", and third string is "explanation".
  // Returns false if i is out of range.
  bool GetTerm(
    int i, 
    std::string* question, 
    std::string* answer, 
    std::string* expl = nullptr) const;

  // Add a vector of associated strings: the strings are localised as they
  //  are stored. A string starting "$$$" will be looked up!
  // Strs must contain at least 2 strings ("question" and "answer").
  void AddTerm(const Strings& strs);

private:
  std::vector<Strings> m_dictionary;
};

Resource* DictionaryLoader(const std::string& resName);
}
