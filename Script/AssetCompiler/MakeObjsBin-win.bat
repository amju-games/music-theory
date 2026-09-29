rem Make binary versions of .obj files

set RESTORE_DIR=%cd%
set TOP_DIR=%RESTORE_DIR%\..\..
set DEST_DIR=%TOP_DIR%\Build\CompiledAssets\Win\obj
set SRC_DIR=%TOP_DIR%\Assets

mkdir %DEST_DIR%

cd %SRC_DIR%\obj\

for %%f in (*.obj) do %RESTORE_DIR%\obj2bin %%f %DEST_DIR%\%%f

cd %RESTORE_DIR%
