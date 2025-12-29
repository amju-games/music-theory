# * Amjula Music Theory *
# (c) Copyright 2024 Juliet Colman

# Test harness for MakeScore
# This is for testing the output from MakeScore.
# We append the output to file scoretest.txt, and load it into
#  the main AmjulaMusicTheory app, in "GS Test Score" mode.

pushd ../../Assets/Gui
cp scoretest.txt gs_test_show_score.txt
popd

./makescore --stave-single --oneline --transpose 2 --page-width 1.0 --scale 1.0 "[Andante] 4/4 clef-t key-s-0 <c> 60 <slur> p 62 < 64 /< 65 f | 67 > 69 71 /> 72 mf </slur>" > out.txt

# Check rhythm stave type works OK
#./makescore --oneline "2/4 <beam> q q </beam> c t | q qr <stacc> <beam> qq qq qq qq </beam>" > out.txt

# Append output to boilerplate
cat out.txt >> ../../Assets/Gui/gs_test_show_score.txt
cat out.txt

# Run the game in 'view score' mode to display the score.
../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

