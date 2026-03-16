// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <algorithm> // transform
#include <File.h>
#include <Localise.h>
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
    if (strs.size() < 2)
    {
      // Not an error, the end of the dictionary
      break;
    }
    AddTerm(strs);
  }
  return true;
}

void Dictionary::AddTerm(const Strings& strs)
{
  // Insert localised strings into this vec
  Strings localised;

  std::transform(strs.cbegin(), strs.cend(), std::back_inserter(localised),
    [](const std::string& s) { return Lookup(s); }
  );

  m_dictionary.push_back(localised);
}

int Dictionary::GetNumTerms() const
{
  return m_dictionary.size();
}

bool Dictionary::GetTerm(int i, std::string* english, std::string* foreign, std::string* expl) const
{
  if (i >= static_cast<int>(m_dictionary.size()))
  {
    return false;
  }

  const Strings& strs = m_dictionary[i];
  Assert(strs.size() >= 2);
  *english = strs[0];
  *foreign = strs[1];
  if (expl != nullptr)
  {
    if (strs.size() > 2)
    {
      *expl = strs[2];
    }
    else
    {
      *expl = "";
    }
  }

  return true;
}
}

