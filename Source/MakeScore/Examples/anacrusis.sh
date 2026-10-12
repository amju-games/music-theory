# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman

# Test anacrusis
../makescore --scale 0.5 "[Anacrusis] page-w 4 3/4 <c> c4 | <m.> d4 |" > out.txt

# Append output to boilerplate
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
cat out.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

