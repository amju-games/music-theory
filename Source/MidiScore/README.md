# MidiScore
Converts midi files into MakeScore input text, for further processing into a score.

Acknowledgements
https://github.com/craigsapp/midifile
Thank you Craig!

To build and run example:
```
make clean
make
./midiscore files/twinkle.txt
```

To build and run tests:
```
cd test
make clean
make
make run
```

TODO
Combine tracks?
Output chords
Output separate staves
Break rests on beats? E.g. a dotted minim rest becomes a crotchet rest + a minim rest: rm. -> cr mr
Add bar lines
User (i.e. me) can specify time sig rather than guessing
Tied notes across bar lines

