# * Amjula Music Theory *
# (c) Copyright 2017 Jason Colman

# Generate "What's the missing rest" questions
# For each line in the 'dictionary' of questions:
# 1. Generate glyphs (including timing and midi 'special glyphs') from simple
#    representation of rhythm
# 2. Append answer to line
# (3. Create explanation GUI file and append filename to line)
#
# NB We also split questions into difficulty levels. Each level is in a 
#  separate dictionary file.
#
# For the explanation, we want to show an animated score and text, so we use
#  a GUI file. 
# E.g. look at tonic triad topic.
# Create the GUI file, and just give the file name.



export OUT=../../Assets/Course/missing_rest.txt
cp boiler.txt $OUT
echo '// Generated from script! Source/MakeScore/make_missing_rest_questions.sh' >> $OUT
echo >> $OUT

export ANS_Q='=rest-quaver,0.5,1,.5,.5'
export ANS_CR='=rest-crotchet,0.5,1,.5,.5'
export ANS_MIN='=rest-minim,0.5,1,.5,.5'

export EXPL_34_CR='=@@@The time signature tell us there are 3 crotchet beats in every bar. So we need a crotchet rest here to give one more beat.'
export EXPL_44_CR='=@@@The time signature tell us there are 4 crotchet beats in every bar. So we need a crotchet rest here to give one more beat.'
export EXPL_44_MIN='=@@@The time signature tell us there are 4 crotchet beats in every bar. We need a minim rest here to give 2 more beats.'

echo '3/4 *cr  c   c | m.' | ./a.out --oneline  >> $OUT
echo $ANS_CR $EXPL_34_CR >> $OUT
echo >> $OUT

echo '4/4 c  c *mr | m c c' | ./a.out --oneline  >> $OUT
echo $ANS_MIN $EXPL_44_MIN >> $OUT
echo >> $OUT


