REM @echo off
REM Run MusicTheory app to display output from MakeScore

REM Run MakeScore, write output to out.txt
echo "4/4 clef-t <c> key-f-4 61 62 63 64 | 65 66 67 70" | ..\..\Build\MakeScore\Debug\MakeScore.exe --stave-single --oneline > out.txt
REM Checking rhythm-only still works as before
REM echo "2/4 q - q c t | q qr qq = qq = qq = qq" | ..\..\Build\MakeScore\Debug\MakeScore.exe --oneline > out.txt
REM echo "4/4 c c m t | c cr mr" | ..\..\Build\MakeScore\Debug\MakeScore.exe --oneline > out.txt

REM Append output to boilerplate GUI file
copy ..\..\Assets\Gui\scoretest.txt ..\..\Assets\Gui\gs_test_show_score.txt
type out.txt >>  ..\..\Assets\Gui\gs_test_show_score.txt

REM Run MusicTheory app - (special build with GSTestShowScore as the active state)
cd ..\..\Assets
start ..\Build\Debug\AmjulaMusicTheory-ShowScore.exe
cd ..\Source\MakeScore