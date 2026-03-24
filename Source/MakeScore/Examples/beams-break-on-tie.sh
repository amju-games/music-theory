# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman

# Some horrible tied beamed notes: we expect beams to be split on
#  beats and every tie.
../makescore --scale 0.7 "page-w 4 [Beams - break on ties] 4/4 <67> q t  qq qq  q t q  qq qq qq t qqq" > out.txt


# Append output to boilerplate
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
cat out.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

