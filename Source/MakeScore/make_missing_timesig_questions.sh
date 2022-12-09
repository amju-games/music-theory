# * Amjula Music Theory *
# (c) Copyright 2017 Jason Colman

# Generate "What's the missing time signature" questions
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



export OUT=../../Assets/Course/missing_time_sig.txt
cp boiler.txt $OUT
echo '// Generated from script! Source/MakeScore/make_missing_timesig_questions.sh' >> $OUT
echo >> $OUT

export ANS_24='=2/4,0.5,1,.5,.5'
export ANS_34='=3/4,0.5,1,.5,.5'
export ANS_44='=4/4,0.5,1,.5,.5'

# These can be GUIs, so we can use symbol for the time sig glyph.
# TODO

export EXPL_24='=@@@There are two crotchet beats in each bar, so the time signature is 2/4.'
export EXPL_34='=@@@There are three crotchet beats in each bar, so the time signature is 3/4.'
export EXPL_44='=@@@There are four crotchet beats in each bar, so the time signature is 4/4.'

# 2/4

echo '*2/4  c cr | c cr | q -- q  c | m' | ./a.out --oneline  >> $OUT
echo $ANS_24 $EXPL_24 >> $OUT
echo >> $OUT

echo '*2/4  c cr | c cr | q -- q  cr | m' | ./a.out --oneline  >> $OUT
echo $ANS_24 $EXPL_24 >> $OUT
echo >> $OUT

echo '*2/4 c c | mr | c c | mr' | ./a.out --oneline  >> $OUT
echo $ANS_24 $EXPL_24 >> $OUT
echo >> $OUT

# 3/4

echo '*3/4 c cr c | m.' | ./a.out --oneline  >> $OUT
echo $ANS_34 $EXPL_34 >> $OUT
echo >> $OUT

echo '*3/4 c m | c m | c c c | m cr' | ./a.out --oneline  >> $OUT
echo $ANS_34 $EXPL_34 >> $OUT
echo >> $OUT

echo '*3/4 c mr | c mr | m.' | ./a.out --oneline  >> $OUT
echo $ANS_34 $EXPL_34 >> $OUT
echo >> $OUT

# 4/4

echo '*4/4 c c c cr | m c c' | ./a.out --oneline  >> $OUT
echo $ANS_44 $EXPL_44 >> $OUT
echo >> $OUT

echo '*4/4 c cr c cr | c c m' | ./a.out --oneline  >> $OUT
echo $ANS_44 $EXPL_44 >> $OUT
echo >> $OUT

echo '*4/4 c c mr | m c c' | ./a.out --oneline  >> $OUT
echo $ANS_44 $EXPL_44 >> $OUT
echo >> $OUT


