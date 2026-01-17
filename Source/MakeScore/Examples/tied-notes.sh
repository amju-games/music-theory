# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman

../makescore "page-w 4 <g4> c t c t m | <c> a4 t a4 t <m> a4" > out.txt

# Append output to boilerplate
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
cat out.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

