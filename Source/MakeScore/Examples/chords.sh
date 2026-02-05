# * Amjula Music Theory *
# (c) Copyright 2024 Juliet Colman

# Test harness for MakeScore
# This is for testing the output from MakeScore.
# We append the output to file scoretest.txt, and load it into
#  the main AmjulaMusicTheory app, in "GS Test Score" mode.

pushd ../../../Assets/Gui
cp scoretest.txt scoretest_temp.txt
popd

../makescore --scale 0.5 "page-w 4 [Chords] ( e4 f4 g4 a4 b4 c5 ) ( e4+ g4+ a4+ ) ( e4+ f4+ a4- ) ( e4+ f4- g4+ a4- ) ( <c> e5 f5+ g5+ ) ( g3 b3 ) ( <sb> c4 <c> e4 <m> g4 ) <c> ( f4 a4 <m> c5 )" > out.txt

# Append output to boilerplate
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
cat out.txt

# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt


