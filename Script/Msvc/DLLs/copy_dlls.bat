REM ************************************************
REM *** Copy DLLs etc that Piano Fest depends on ***
REM ************************************************

REM These DLLs are copied from amjulib/3rdParty.
REM We are now building for x64 so these are x64 DLLs!

REM this is the working dir for Release/Glue builds
set RELEASE_DIR=..\..\..\Build\CompiledAssets

REM this is the working dir for Debug/No glue builds
set DEBUG_DIR=..\..\..\Assets

REM Make sure release build folder exists. 
mkdir %RELEASE_DIR%


REM **************
REM ***  BASS  ***
REM **************

set BASS_DIR=%cd%\..\..\..\..\amjulib\3rdPartyLibs\Bass\2.4.18.3\bass24-win\x64\
set BASS_MIDI_DIR=%cd%\..\..\..\..\amjulib\3rdPartyLibs\Bass\BassMidi\2.4.15.3\bassmidi24-win\x64\

copy %BASS_DIR%\*.dll       %RELEASE_DIR%
copy %BASS_MIDI_DIR%\*.dll  %RELEASE_DIR%

copy %BASS_DIR%\*.dll       %DEBUG_DIR%
copy %BASS_MIDI_DIR%\*.dll  %DEBUG_DIR%


REM ******************
REM ***  BugSplat  ***
REM ******************

set BUGSPLAT_RELEASE_DIR=%cd%\..\..\..\..\amjulib\3rdPartyLibs\BugSplat\Windows\x64\Debug\bin\
set BUGSPLAT_DEBUG_DIR=%cd%\..\..\..\..\amjulib\3rdPartyLibs\BugSplat\Windows\x64\Debug\bin\

copy %BUGSPLAT_RELEASE_DIR%\BugSplatRc.dll       %RELEASE_DIR%
copy %BUGSPLAT_RELEASE_DIR%\BugSplatMonitor.exe  %RELEASE_DIR%

copy %BUGSPLAT_DEBUG_DIR%\BugSplatRc.dll       %DEBUG_DIR%
copy %BUGSPLAT_DEBUG_DIR%\BugSplatMonitor.exe  %DEBUG_DIR%




