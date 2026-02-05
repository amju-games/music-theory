# * Amjula Music Theory *
# (c) Copyright 2026 Juliet Colman


../makescore "page-w 4 [Rests] <qq> r <q> r <c> r <m> r <sb> r | <qq.> r <q.> r <c.> r <m.> r <sb.> r" > out.txt

cat out.txt

# Append output to boilerplate
# TODO We can load score from out.txt now
cp ../../../Assets/Gui/scoretest.txt ../../../Assets/Gui/scoretest_temp.txt
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt

