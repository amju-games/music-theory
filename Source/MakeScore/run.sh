# * Amjula Music Theory *
# (c) Copyright 2017 Jason Colman

# Test harness for MakeScore
# This is for testing the output from MakeScore.
# We append the output to file scoretest.txt, and load it into
#  the main AmjulaMusicTheory app, in "GS Test Score" mode.

pushd ../../Assets/Gui
cp scoretest.txt gs_test_show_score.txt
popd

echo '2/4 q -- q c t | q qr qq == qq == qq == qq' | ./a.out --oneline  > out.txt

# Append output to boilerplate
cat out.txt >> ../../Assets/Gui/gs_test_show_score.txt
cat out.txt

pushd ../../Script/Macosx
make run
popd


