# * Amjula Music Theory *
# (c) Copyright 2017 Jason Colman

# Test MakeScore

pushd ../../Assets/Gui
cp scoretest.txt gs_test_show_score.txt
popd

echo '3/4 *cr  c   c' | ./a.out --oneline  > out.txt

# Shows that we can append answer etc to the same line
#echo '=@@@Answer' >> out.txt

cat out.txt >> ../../Assets/Gui/gs_test_show_score.txt
cat out.txt

pushd ../../Script/Macosx
make run
popd


