#include "MyROConfig.h"

namespace Amju
{
RCPtr<ConfigFile> s_config = nullptr;

ConfigFile* MyROConfig()
{
  if (s_config == nullptr)
  {
    s_config = new ConfigFile;
    ReloadMyROConfig();
  }

  return s_config;
}

// Reload, so we can make changes at run time.
void ReloadMyROConfig()
{
  auto ro = MyROConfig();
  ro->Clear();
  if (!ro->Load("roconfig.txt"))
  {
    Assert(0);
  }
}
}

