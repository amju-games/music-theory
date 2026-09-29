REM Make PIANO FEST Inno Setup distribution for Windows
REM (c) Copyright Juliet Colman 2026

REM run from Script\WindowsInstaller

REM **********************
REM * Build exe and data *
REM **********************

CALL BuildWindowsRelease.bat

set TOP_DIR=%cd%\..\..

REM ***********************************
REM * Copy all distro files to folder *
REM ***********************************

cd %TOP_DIR%

SET DEST=Build\WindowsItchio

MKDIR %DEST%
COPY Build\Release\amju_piano_fest.exe %DEST%

REM Important for itch.io app! Identifies game exe.
COPY Script\WindowsInstaller\.itch.toml %DEST%

COPY Build\CompiledAssets\data-WIN.glue %DEST%
COPY Build\CompiledAssets\music-WIN.glue %DEST%

REM BASS DLLs
COPY ..\amjulib\3rdPartyLibs\Bass\2.4.18.3\bass24-win\x64\bass.dll %DEST%
COPY ..\amjulib\3rdPartyLibs\Bass\BassMidi\2.4.15.3\bassmidi24-win\x64\bassmidi.dll %DEST%

REM BugSplat binaries
COPY ..\amjulib\3rdPartyLibs\BugSplat\Windows\x64\Release\bin\BugSplatMonitor.exe %DEST%
REM We aren't setting the required registry key for this.
REM COPY ..\amjulib\3rdPartyLibs\BugSplat\Windows\x64\Release\bin\BugSplatWer.dll %DEST%
COPY ..\amjulib\3rdPartyLibs\BugSplat\Windows\x64\Release\bin\BugSplatRc.dll %DEST%

REM MS redistributables, also required by BugSplat.
XCOPY ..\amjulib\3rdPartyLibs\BugSplat\Windows\Redist_DLLs\*.dll %DEST% /y


REM Distro folder is Build\WindowsItchio

