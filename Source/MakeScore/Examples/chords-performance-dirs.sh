# * Amjula Music Theory *
# (c) Copyright 2024 Juliet Colman

# Test harness for MakeScore
# This is for testing the output from MakeScore.
# We append the output to file scoretest.txt, and load it into
#  the main AmjulaMusicTheory app, in "GS Test Score" mode.

pushd ../../../Assets/Gui
cp scoretest.txt scoretest_temp.txt
popd

../makescore --scale 0.5 "page-w 4 [Chords - Performance directions] <q> ( e4 p f4 g4 a4 b4 c5 ) <c> ( e4+ g4+ mp a4+ ) <m>  ( e4+ f4+ a4- ) mf <sb> ( e4+ f4- g4+ a4- f ) |" > out.txt; 

# Append output to boilerplate
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
cat out.txt

# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt


