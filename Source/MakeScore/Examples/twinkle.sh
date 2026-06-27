# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman

../makescore --scale 0.5 "page-w 4 stave clef-t 4/4  key-s-2 <c> 72 mf 72 79 79 | <c> 81 81 <m> 79 | <c> 77 77 76 76 | <c> 74 74 <m> 72 | stave clef-b 4/4  key-s-0 <c> 48 mf 60 64 60 | <c> 65 60 64 60 | clef-t <c> 62 59 60 57 | <c> 53 55 <m> 48 | " > out.txt

cat out.txt

# Append output to boilerplate
# TODO We can load score from out.txt now
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

