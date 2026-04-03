#pragma once

#include <ROConfig.h>

namespace Amju
{
// ROConfig is a read-only config file, so we can change
//  consts without rebuilding. It is copied from glue file to
//  the save dir so that it can be overwritten by downloading
//  from a server.
// But in this game, there is no server side, and it's simpler
//  to load the config from glue file once.
ConfigFile* MyROConfig();

// Reload, so we can make changes at run time.
void ReloadMyROConfig();
}

