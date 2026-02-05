# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman


../makescore --scale 0.7 "page-w 4 [Note lengths] <qq> e5- <q> d5- <c> c5 <m> b4- <sb> a4-  <q.> d4 <c.> e4 <m.> f4 <sb.> c5 d5" > out.txt

cat out.txt

# Append output to boilerplate
# TODO We can load score from out.txt now
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

