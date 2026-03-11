# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman

# Test width scaling
../makescore --scale 0.5 "page-w 4 4/4 <sb> r | <qq> g3 a3 b3 c4 d4 e4 f4+ g4 a4 b4 c5 d5 e5 f5+ <q> g5 | t sb |" > out.txt

# Append output to boilerplate
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
cat out.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

