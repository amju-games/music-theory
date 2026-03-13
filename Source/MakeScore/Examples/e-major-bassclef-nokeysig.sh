# * Amjula Music Theory *
# (c) Copyright 2024 Juliet Colman

pushd ../../../Assets/Gui
cp scoretest.txt scoretest_temp.txt
popd

# Test accidentals on bass clef
../makescore --scale 0.8 "[Accidentals, bass clef] page-w 4 <c> clef-b 4/4 e3 f3+ g3+ a3 | b3 c4+ d4+ e4" > out.txt

# Append output to boilerplate
cat out.txt >> ../../../Assets/Gui/scoretest_temp.txt
cat out.txt

# Run the game in 'view score' mode to display the score.
../../../Build/Mac/macgame --viewscore Gui/scoretest_temp.txt


