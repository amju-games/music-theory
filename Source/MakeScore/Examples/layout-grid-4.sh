# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman

# Test width scaling
../makescore --scale 0.5 "[Layout Grid 4] page-w 4 4/4 <sb> g3 | <m> c4 c5 | <c> c4 e4 g4 c5 | <m> g4 b4 | <sb> c5 |" > out.txt

# Append output to boilerplate
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
cat out.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

