# * Amjula Music Theory *
# (c) Copyright 2017 Jason Colman

# Test MakeScore

pushd ../../Assets/Gui
cp scoretest.txt gs_test_show_score.txt
popd

echo '3/4 q -- q c  c t | c c cr' | ./a.out  > out.txt

cat out.txt >> ../../Assets/Gui/gs_test_show_score.txt
cat out.txt

pushd ../../Script/Macosx
make run
popd


