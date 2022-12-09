REM @echo off
REM Run MusicTheory app to display output from MakeScore

REM Run MakeScore, write output to out.txt
echo "4/4 clef-t <c.> 48 50 <q> 52 -- 53 | <c> 55 57 59 cr | <sb> 74 76 77 79 | 82 84 86 88" | ..\..\Build\MakeScore\Debug\MakeScore.exe --stave-single --oneline > out.txt
Rem Checking rhythm-only still works as before
REM echo "2/4 q - q c t | q qr qq = qq = qq = qq" | ..\..\Build\MakeScore\Debug\MakeScore.exe --oneline > out.txt

REM Append output to boilerplate GUI file
copy ..\..\Assets\Gui\scoretest.txt ..\..\Assets\Gui\gs_test_show_score.txt
type out.txt >>  ..\..\Assets\Gui\gs_test_show_score.txt

REM Run MusicTheory app - (special build with GSTestShowScore as the active state)
cd ..\..\Assets
start ..\Build\Debug\AmjulaMusicTheory-ShowScore.exe
cd ..\Source\MakeScore