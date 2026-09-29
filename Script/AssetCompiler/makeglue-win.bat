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
set "GLUE_FILE=%DEST_DIR%\..\data-%PLATFORM%.glue"

:: 2. Create all directories at once
for %%d in (font2d Image md2 obj Scene Shaders\gles Shaders\opengl Sound\wav Songs) do (
    if not exist "%DEST_DIR%\%%d" mkdir "%DEST_DIR%\%%d"
)

:: 3. Copy Assets
xcopy "%SRC_DIR%\*.txt" "%DEST_DIR%\" /y
xcopy "%SRC_DIR%\font2d\*" "%DEST_DIR%\font2d\" /e /y
xcopy "%SRC_DIR%\Gui\*" "%DEST_DIR%\Gui\" /e /y
xcopy "%SRC_DIR%\Songs\*" "%DEST_DIR%\Songs\" /e /y
xcopy "%SRC_DIR%\md2\*" "%DEST_DIR%\md2\" /e /y
xcopy "%SRC_DIR%\Image\*.png" "%DEST_DIR%\Image\" /y
xcopy "%SRC_DIR%\Shaders\gles\*.txt" "%DEST_DIR%\Shaders\gles\" /y
xcopy "%SRC_DIR%\Shaders\opengl\*.txt" "%DEST_DIR%\Shaders\opengl\" /y
xcopy "%SRC_DIR%\Scene\*.txt" "%DEST_DIR%\Scene\" /y
xcopy "%SRC_DIR%\obj\*.png" "%DEST_DIR%\obj\" /y
xcopy "%SRC_DIR%\obj\*.mtl" "%DEST_DIR%\obj\" /y
xcopy "%SRC_DIR%\obj\*.png" "%DEST_DIR%\" /y

:: 4. Add EOL to all text/csv files
for /r "%DEST_DIR%" %%f in (*.txt *.csv) do (
    echo(>>"%%f"
)

:: 5. Create the Glue File
cd /d "%DEST_DIR%"
"%GLUE_EXE%" -c "%GLUE_FILE%"

:: 6. Add all files to Glue
for /r %%f in (*.txt *.csv *.png *.obj *.mtl) do (
    set "FULL_PATH=%%f"
    :: Strip the DEST_DIR path to get the relative path
    set "REL_PATH=!FULL_PATH:%DEST_DIR%\=!"
    echo Adding file: !REL_PATH!
    "%GLUE_EXE%" -a "%GLUE_FILE%" "!REL_PATH!"
)

for /r "%DEST_DIR%\md2" %%f in (*.md2) do (
    set "FULL_PATH=%%f"
    set "REL_PATH=!FULL_PATH:%DEST_DIR%\=!"
    echo Adding file: !REL_PATH!
    "%GLUE_EXE%" -a "%GLUE_FILE%" "!REL_PATH!"
)

:: 7. Verify and Return
"%GLUE_EXE%" -d "%GLUE_FILE%"
cd /d "%THIS_DIR%"

endlocal
