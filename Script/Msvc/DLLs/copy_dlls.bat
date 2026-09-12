REM These DLLs are copied from amjulib/3rdParty.
REM We are now building for x64 so these are the x64 DLLs!

mkdir ..\..\..\Build\CompiledAssets

REM this is the working dir for Release/Glue builds
copy *.dll ..\..\..\Build\CompiledAssets

REM this is the working dir for Debug/No glue builds
copy *.dll ..\..\..\Assets
