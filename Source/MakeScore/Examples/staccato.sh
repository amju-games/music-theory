# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman


../makescore --scale 0.5 "page-w 4 [Staccato] <c> c4. e4. g4. ( c4. e4 g4 c5 )" > out.txt

cat out.txt

# Append output to boilerplate
# TODO We can load score from out.txt now
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

