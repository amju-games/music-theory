# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman


../makescore --scale 0.5 "page-w 4 [Rests] 4/4 <qq> r r <q> r <c> r <m> r | <sb> r | stave 4/4 <m> r <c> r <q> r <qq> r r | <sb> R" > out.txt

cat out.txt

# Append output to boilerplate
# TODO We can load score from out.txt now
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

