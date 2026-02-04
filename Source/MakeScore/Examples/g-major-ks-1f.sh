# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman

# G major scale but with F major key sig.
# We get b natural and f# accidentals.
../makescore --scale 0.5 "page-w 5 <c> key-f-1 g3 a3 b3 c4 d4 e4 f4+ g4 a4 b4 c5 d5 e5 f5+ g5" > out.txt

# Append output to boilerplate
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
cat out.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

