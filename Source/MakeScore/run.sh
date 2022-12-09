# * Amjula Music Theory *
# (c) Copyright 2017 Jason Colman

# Test MakeScore

pushd ../../Assets/Gui
cp scoretest.txt gs_test_show_score.txt
popd

echo 'c t c t c' | ./a.out --oneline  > out.txt

cat out.txt >> ../../Assets/Gui/gs_test_show_score.txt
cat out.txt

pushd ../../Script/Macosx
make run
popd


