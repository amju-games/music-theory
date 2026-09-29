REM Copy soundfonts to Build/CompiledAssets, where we look for glue files.
REM This is for Win Release/Glue builds running from MSVC.

mkdir ..\..\Build\CompiledAssets\Sound
xcopy ..\..\Assets\Sound\*.sf2 ..\..\Build\CompiledAssets\Sound
