# Run this to rebuild the main functions for each exe.
# Depending on #defines we build the game main(), listen main() or catch main().
# Do this when switching from building one exe to another. 
# Example usage:
#
#   make  <-- builds game
#   ...
#   make  <-- build it again
#   < ...i.e. we can build game multiple times >
#
#   < Now I want to build Listen >
#
#   ./touch_main_cpps.sh   <-- this script
#   make -f ListenMakefile   <-- build Listen
# 
#   < Now I want to build the tests>
#   ./touch_main_cpps.sh   <-- run this script again
#   make -f TestsMakefile

touch ../../Source/GameMain.cpp
touch ../../Source/Listen/ListenMain.cpp
touch ../../Source/Tests/CatchTestMain.cpp

