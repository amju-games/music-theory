REM @echo off
REM Run MusicTheory app to display output from MakeScore

REM Run MakeScore, write output to out.txt

echo "4/4 clef-t [Andante] <c> key-f-4 61 <slur> p 62 < 73 74 | 65 80 </slur> <stacc> 67 /< 70 mf " | C:\Users\Juliet\repos\amt\music-theory\Script\Msvc\AmjulaMusicTheory\x64\Debug\MakeScore.exe --stave-single > out.txt

REM echo "4/4 clef-t [Andante] <c> key-f-4 61 <slur> p 62 < 73 74 | 65 80 </slur> <stacc> 67 /< 70 mf " | ..\..\Build\MakeScore\Debug\MakeScore.exe --stave-single > out.txt
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