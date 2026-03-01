# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman

../makescore --scale 0.5 "page-w 4 [Beams, chords] <q> ( g3 g4 ) (  a3 c4 ) ( b3 d4 )  <c> c4 <q> c5 g4 e4 <c> c4 <qq> c5 d5 c5 d5 <c> c5" > out.txt

#c4 b3 g3 d3 d4 e4 f4+ g4 a4 b4 c5 d5 e5 f5+ g5" > out.txt

# Append output to boilerplate
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
cat out.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

