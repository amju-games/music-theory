# Pull request checklist

## Localisation
We should do localisation tasks on the branch where new strings are added, because master should always be in a releasable state after every commit.
 - Run localisation script over GUI and source.
```
cd repo-top-dir
perl ../amjulib/Source/Localise/localise.pl Assets/en.txt Assets
perl ../amjulib/Source/Localise/localise.pl Assets/en.txt Source/*.cpp
```
 - Add new strings to language files and translate.
 - Test the new strings in game.

## Merge master into branch
 - Do this if there have been other commits to master.
 - Rebuild tests and game.

## Rebuild tests
 - Make sure the tests are up-to-date.
```
cd Script/Macosx
./touch_main_cpps.sh
make -f TestsMakefile
```

