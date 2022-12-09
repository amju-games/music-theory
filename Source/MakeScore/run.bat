REM @echo off
REM Run MusicTheory app to display output from MakeScore

REM Run MakeScore, write output to out.txt
echo "4/4 <c> 48 50 52 53 | 55 57 59 cr <c> | 60 62 64 65 | 67 69 71 72" | ..\..\Build\MakeScore\Debug\MakeScore.exe --stave-single --oneline > out.txt

REM Append output to boilerplate GUI file
copy ..\..\Assets\Gui\scoretest.txt ..\..\Assets\Gui\gs_test_show_score.txt
type out.txt >>  ..\..\Assets\Gui\gs_test_show_score.txt

REM Run MusicTheory app - (special build with GSTestShowScore as the active state)
cd ..\..\Assets
start ..\Build\Debug\AmjulaMusicTheory-ShowScore.exe
cd ..\Source\MakeScore