# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman

# Test width scaling due to accidentals
../makescore --scale 0.5 "[Widths - accidentals] page-w 4 4/4 <c> g4 g4 g4 g4 | g4 g4+ g4- g4 | ( g4 b4 d5 ) ( g4+ b4+ d5- ) c5 c5 " > out.txt

# Append output to boilerplate
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
cat out.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

