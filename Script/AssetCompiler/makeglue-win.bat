REM makeglue.bat
REM glue every file (which we want) in this directory into a Glue file.

set THIS_DIR=%cd%
set TOP_DIR=%THIS_DIR%\..\..
set COMPILED_ASSETS=%TOP_DIR%\Build\CompiledAssets
set DEST_DIR=%COMPILED_ASSETS%\Win
set SRC_DIR=%TOP_DIR%\Assets
set SRC_FONT2D=%TOP_DIR%\..\amjulib\Assets\font2d

mkdir %COMPILED_ASSETS%
mkdir %DEST_DIR%
mkdir %DEST_DIR%\font2d
rem del /S /Q %DEST_DIR%\*.*

REM Copy other files to compiled dir
REM /Y when we would get overwrite confirm prompt

xcopy /Y /S %SRC_DIR%\*.txt %DEST_DIR%
xcopy /Y /S %SRC_DIR%\obj\*.txt %DEST_DIR%
xcopy /Y /S %SRC_DIR%\gui\common\*.txt %DEST_DIR%
xcopy /Y /S %SRC_DIR%\gui\win\*.txt %DEST_DIR%
xcopy /Y /S %SRC_FONT2D%\* %DEST_DIR%\font2d\
xcopy /Y /S %SRC_DIR%\tex\common\*.png %DEST_DIR%
xcopy /Y /S %SRC_DIR%\tex\win\*.png %DEST_DIR%
xcopy /Y /S %SRC_DIR%\md2\*.md2 %DEST_DIR%

cd %DEST_DIR%

set GLUE_EXE=%THIS_DIR%\glue.exe
set GLUE_FILE=%DEST_DIR%\..\data-win.glue
REM make glue file
%GLUE_EXE% -c %GLUE_FILE%

REM for each file, add to glue file.
for %%f in (*.txt, *.md2) do %GLUE_EXE% -a %GLUE_FILE% %%f

REM texture files
for %%f in (*.png) do %GLUE_EXE% -a %GLUE_FILE% %%f

REM obj files are NOT in sub dir
for %%f in (*.obj) do %GLUE_EXE% -a %GLUE_FILE% %%f

REM Add files in font3d dir
for %%f in (obj\font3d\*.obj) do %GLUE_EXE% -a %GLUE_FILE% %%f

REM Add files in font2d dir
for %%f in (font2d\*.txt, font2d\*.png) do %GLUE_EXE% -a %GLUE_FILE% %%f

REM Add files in levels dir
for %%f in (levels\*.txt) do %GLUE_EXE% -a %GLUE_FILE% %%f


REM Verify contents
%GLUE_EXE% -d %GLUE_FILE%


cd %THIS_DIR%

