# * Amjula Music Theory *
# (c) Copyright 2017 Jason Colman

# Test harness for MakeScore
# This is for testing the output from MakeScore.
# We append the output to file scoretest.txt, and load it into
#  the main AmjulaMusicTheory app, in "GS Test Score" mode.

pushd ../../Assets/Gui
cp scoretest.txt gs_test_show_score.txt
popd

#echo '[Andante] 4/4 clef-t key-s-0 <c> 60 <slur> p 62 < 64 /< 65 f | 67 > 69 71 /> 72 mf </slur>' | ./makescore --stave-single --oneline --transpose 2 --page-width 1.0 --scale 1.0 > out.txt

# Check rhythm stave type works OK
#echo '4/4 m c c' | ./makescore --oneline  > out.txt
#echo "2/4 'thing' q -- q c t | q qr <stacc> qq = qq == qq == qq" | ./makescore --oneline  > out.txt
# New beaming tokens
echo "2/4 <beam> q q </beam> c t | q qr <stacc> <beam> qq qq qq qq </beam>" | ./makescore --oneline  > out.txt

# Append output to boilerplate
cat out.txt >> ../../Assets/Gui/gs_test_show_score.txt
cat out.txt

pushd ../../Script/Macosx
make run
popd


