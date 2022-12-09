// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <ConfigFile.h>
#include "Hints.h"
#include "Keys.h"
#include "UserProfile.h"

namespace Amju
{
static const char* HINTS_AVAILABLE_KEY = "hints_avail";
// Start life with 3 hints
const int DEFAULT_HINTS_AVAIL = 3;

int Hints::Get()
{
  auto userConfig = TheUserProfile()->GetConfigForTopic(KEY_GENERAL);
  return userConfig->GetInt(HINTS_AVAILABLE_KEY, DEFAULT_HINTS_AVAIL);
}

void Hints::Inc()
{
  auto userConfig = TheUserProfile()->GetConfigForTopic(KEY_GENERAL);
  int hints = userConfig->GetInt(HINTS_AVAILABLE_KEY, DEFAULT_HINTS_AVAIL);
  hints++;
  userConfig->SetInt(HINTS_AVAILABLE_KEY, hints);
  TheUserProfile()->Save();
}

void Hints::Dec()
{
  auto userConfig = TheUserProfile()->GetConfigForTopic(KEY_GENERAL);
  int hints = userConfig->GetInt(HINTS_AVAILABLE_KEY, DEFAULT_HINTS_AVAIL);
  hints--;
  userConfig->SetInt(HINTS_AVAILABLE_KEY, hints);
  TheUserProfile()->Save();
}
}

