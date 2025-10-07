// * Amjula music theory *
// (c) Copyright 2017 Juliet Colman

#include "CreateDictionary.h"

using namespace Amju;

Dictionary* CreateDic()
{
  Dictionary* dic = new Dictionary;

  dic->AddTerm({ "Soft", "Piano" });
  dic->AddTerm({ "Loud", "Forte" });
  dic->AddTerm({ "Sweetly", "Dolce" });
  dic->AddTerm({ "Gracefully", "Grazioso" });

  return dic;
}

Dictionary* CreateDicWithDuplicates()
{
  Dictionary* dic = CreateDic();

  dic->AddTerm({ "p", "Piano" });
  dic->AddTerm({ "p", "Soft" });

  return dic;
}
