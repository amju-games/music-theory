* MakeScore *

Part of Amjula Music School 
(c) Jason Colman 2017-2018

MakeScore converts simple strings describing music notation
into more complicated strings. 

List of input tokens
Time values
sb  - Semibreve
m   - Minim
c   - Crotchet
q   - Quaver
qq  - Semiquaver

Appending 'r' means 'rest'. Appending '.' makes the note or rest
dotted. E.g.:
m.  - Dotted minim
cr. - Dotted crotchet rest

Time signature input tokens
2/4
3/4
4/4
common
cut-common

Key signature input tokens
TODO

Other input tokens
|   - Bar line
*   - This is used to hide a note or rest and replace it with an asterisk.
      This can be used to make questions along the lines of 'what is the
      missing note?'

Tied notes
t   - Adds a tie between the previous and next note

Beams
Beam tokens are added between quaver tokens
--  - Single beam, i.e. quaver
==  - Double beam, i.e. semiquaver
-=  - Quaver beamed to semiquaver
=-  - Semiquaver beamed to quaver

Command-line options
--oneline  
Write the entire output on one line with no \n characters. This is used
to make one unbroken string to add to a dictionary file etc.

Usage examples
Interactive:
./makescore
c cr  <- Type this to std in
This will write the output string to std out and quit.

Scripted:
echo '3/4 c *cr c t | m.' | ./makescore --oneline  >> file.txt
Appends output string to file.txt. --oneline is used so the output is one 
unbroken string.

Full test, including rendering
This relies on the main AmjulaMusicTheory app. You need to build it in
"GS Test Score" mode.
run.sh runs MakeScore, then displays the output in the app.
Edit run.sh to change the input to MakeScore.


