# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman


../makescore --scale 0.7 "page-w 4 [Tails] <qq> a5- <q> c5- <qq.> f4 <q.> d4 ( c4 e4 g4 c5 ) ( a3 c4+ e4 a4 )" > out.txt

cat out.txt

# Append output to boilerplate
# TODO We can load score from out.txt now
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

