REM Make PIANO FEST Inno Setup distribution for Windows
REM (c) Copyright Juliet Colman 2026

REM run from Script\WindowsInstaller

REM **********************
REM * Build exe and data *
REM **********************

CALL BuildWindowsRelease.bat

set TOP_DIR=%cd%\..\..

REM ***************************
REM *** Build setup program ***
REM ***************************

cd %TOP_DIR%
"\Program Files\Inno Setup 7\iscc.exe" Script\WindowsInstaller\amju_piano_fest_inno_setup_script.iss

REM Installer is in: Build\WindowsInstaller\amju_piano_fest_installer.exe
