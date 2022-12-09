// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <File.h>
#include <StringUtils.h>
#include "Dictionary.h"

namespace Amju
{
Resource* DictionaryLoader(const std::string& resName)
{
  Dictionary* d = new Dictionary;
  // Remove final extension
  std::string filename = GetFileNoExt(resName);
  if (!d->Load(filename))
  {
    delete d; // TODO res loaders like this should return an RCPtr
    return nullptr;
  }
  return d;
}

bool Dictionary::Load(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    return false;
  }
  std::string line;
  while (f.GetDataLine(&line))
  {
    // Split English/foreign strings 
    Strings strs = Split(line, '=');
    if (strs.size() != 2)
    {
      // Not sure whether to make this an error
      f.ReportError("Finished loading dictionary: " + line);
      break;
    }
    AddTerm(strs[0], strs[1]);
  }
  return true;
}

void Dictionary::AddTerm(const std::string& english, const std::string& foreign)
{
  m_dictionary.push_back(std::make_pair(english, foreign));
}

int Dictionary::GetNumTerms() const
{
  return m_dictionary.size();
}

void Dictionary::GetTerm(int i, std::string* english, std::string* foreign) const
{
  const auto& p = m_dictionary[i];
  *english = p.first;
  *foreign = p.second;
}
}

