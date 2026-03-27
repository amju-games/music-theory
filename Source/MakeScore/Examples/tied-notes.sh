# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman


../makescore --scale 0.8 "page-w 5 [Tied notes] <d4> c t m t c | <g5> sb t c " > out.txt

cat out.txt

# Append output to boilerplate
# TODO We can load score from out.txt now
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

