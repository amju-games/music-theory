REM makemusicglue.bat
REM glue sound files into a Glue file.

set THIS_DIR=%cd%
set TOP_DIR=%THIS_DIR%\..\..
set DEST_DIR=%TOP_DIR%\Build\CompiledAssets\Win
set SRC_DIR=%TOP_DIR%\Assets\sound

mkdir %DEST_DIR%
mkdir %DEST_DIR%\sound


xcopy /S /Y %SRC_DIR%\windows\*.wav %DEST_DIR%\sound
xcopy /S /Y %SRC_DIR%\*.it %DEST_DIR%\sound


cd %DEST_DIR%

set GLUE_EXE=%THIS_DIR%\glue.exe
set GLUE_FILE=%DEST_DIR%\..\music-win.glue
REM make glue file
%GLUE_EXE% -c %GLUE_FILE%

REM Add files in sound dir

for %%f in (sound\*.wav) do %GLUE_EXE% -a %GLUE_FILE% %%f

for %%f in (sound\*.it) do %GLUE_EXE% -a %GLUE_FILE% %%f


REM Verify contents
%GLUE_EXE% -d %GLUE_FILE%


cd %THIS_DIR%
