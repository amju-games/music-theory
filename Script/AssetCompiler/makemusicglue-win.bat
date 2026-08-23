@echo off
setlocal enabledelayedexpansion

:: 1. Setup paths
set "PLATFORM=WIN"
set "THIS_DIR=%cd%"

:: Navigate up to get the TOP_DIR path accurately
pushd "%THIS_DIR%\..\.."
set "TOP_DIR=%cd%"
popd

set "COMPILED_ASSETS=%TOP_DIR%\Build\CompiledAssets"
set "DEST_DIR=%COMPILED_ASSETS%\%PLATFORM%"
set "SRC_DIR=%TOP_DIR%\Assets"
set "GLUE_EXE=%THIS_DIR%\glue.exe"
set "GLUE_FILE=%DEST_DIR%\..\music-%PLATFORM%.glue"

:: 2. Create directories
if not exist "%COMPILED_ASSETS%" mkdir "%COMPILED_ASSETS%"
if not exist "%DEST_DIR%\Sound\wav" mkdir "%DEST_DIR%\Sound\wav"

:: 3. Clear existing files and copy new ones
del /q "%DEST_DIR%\Sound\wav\*.wav" 2>nul
xcopy "%SRC_DIR%\Sound\wav\*.wav" "%DEST_DIR%\Sound\wav\" /y

:: Delete all .mid files recursively in the destination Songs directory
del /s /q "%DEST_DIR%\Songs\*.mid" 2>nul
xcopy "%SRC_DIR%\Songs\*" "%DEST_DIR%\Songs\" /e /y

:: 4. Navigate to destination
cd /d "%DEST_DIR%"

:: 5. Create empty glue file
"%GLUE_EXE%" -c "%GLUE_FILE%"

:: 6. Add WAV files to Glue
for /r "Sound\wav" %%f in (*.wav) do (
    set "FULL_PATH=%%f"
    :: Strip the DEST_DIR path to get the relative path
    set "REL_PATH=!FULL_PATH:%DEST_DIR%\=!"
    echo Adding file: !REL_PATH!
    "%GLUE_EXE%" -a "%GLUE_FILE%" "!REL_PATH!"
)

:: 7. Add MID files to Glue (handles nested folders like Zsh's **)
for /r "Songs" %%f in (*.mid) do (
    set "FULL_PATH=%%f"
    set "REL_PATH=!FULL_PATH:%DEST_DIR%\=!"
    echo Adding file: !REL_PATH!
    "%GLUE_EXE%" -a "%GLUE_FILE%" "!REL_PATH!"
)

:: 8. Verify and Return
"%GLUE_EXE%" -d "%GLUE_FILE%"
cd /d "%THIS_DIR%"

endlocal
