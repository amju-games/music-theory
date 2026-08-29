#pragma once

// * Windows Version *
// This file is included in the .rc file, and used to build the Version Resource.
// This version number for windows only.

#define VERSION_MAJOR               0
#define VERSION_MINOR               4
#define VERSION_REVISION            0
#define VERSION_BUILD               1

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

#define VERSION_NUMBER_STR          STRINGIZE(VERSION_MAJOR) "." \
                                    STRINGIZE(VERSION_MINOR) "." \
                                    STRINGIZE(VERSION_REVISION) "." \
                                    STRINGIZE(VERSION_BUILD)
