// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <string>
#include <vector>
#include <ResourceManager.h>

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
  void GetTerm(int i, std::string* english, std::string* foreign) const;
  void AddTerm(const std::string& english, const std::string& foreign);

private:
  std::vector<std::pair<std::string, std::string>> m_dictionary;
};

Resource* DictionaryLoader(const std::string& resName);
}
