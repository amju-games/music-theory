# * Amjula Music Theory *
# (c) Copyright 2024 Juliet Colman

# Test harness for MakeScore
# This is for testing the output from MakeScore.
# We append the output to file scoretest.txt, and load it into
#  the main AmjulaMusicTheory app, in "GS Test Score" mode.

pushd ../../../Assets/Gui
cp scoretest.txt scoretest_temp.txt
popd

../makescore "page-w 6 <c> clef-b e3 f3+ g3+ a3 b3 c4+ d4+ e4" > out.txt

# Append output to boilerplate
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
cat out.txt

# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt


