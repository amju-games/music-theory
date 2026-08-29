REM Make PIANO FEST distribution for Windows
REM (c) Copyright Juliet Colman 2026

REM Open a command prompt with: Start Menu -> Developer Command Prompt
REM Maybe we should run 'vcvarsall.bat'? 

REM run from Script\WindowsInstaller

set TOP_DIR=%cd%\..\..

REM **********************
REM *** Build game exe ***
REM **********************

cd %TOP_DIR%\Script\Msvc\AmjulaMusicTheory\

msbuild AmjulaMusicTheory.sln /p:Configuration=Release /t:Rebuild /m

REM ...that creates Build/Release/amju_piano_fest.exe

REM ************************
REM *** Build glue files ***
REM ************************

cd %TOP_DIR%

cd Script/AssetCompiler
REM use CALL so we come back here afterwards!
CALL make-everything-win.bat

REM ***************************
REM *** Build setup program ***
REM ***************************

cd %TOP_DIR%
"\Program Files\Inno Setup 7\iscc.exe" Script\WindowsInstaller\amju_piano_fest_inno_setup_script.iss

REM Installer is in: Build\WindowsInstaller\amju_piano_fest_installer.exe
