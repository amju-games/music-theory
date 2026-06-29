# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman

# Test width scaling
../makescore --scale 0.5 "[Layout Grid 5] page-w 4.75 4/4 <sb> c5 | <qq> 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 |" > out.txt

# Append output to boilerplate
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
cat out.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

