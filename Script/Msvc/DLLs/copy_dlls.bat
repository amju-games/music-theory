REM These DLLs are copied from amjulin/3rdParty.
REM We are now building for x64 so these are the x64 DLLs!

mkdir ..\..\..\Build\Debug
mkdir ..\..\..\Build\Release
mkdir ..\..\..\Build\Catch

copy *.dll ..\..\..\Build\Debug
copy *.dll ..\..\..\Build\Release
copy *.dll ..\..\..\Build\Catch
REM this is the workding dir for Release/Glue builds
copy *.dll ..\..\..\Build\CompiledAssets
