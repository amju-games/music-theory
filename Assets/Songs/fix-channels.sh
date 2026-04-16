#!/bin/bash

# MIDI files exported from Studio One sadly do not respect channel
#  mappings, with all channels set to zero. 
# This script uses midicsv to fix the channels for each track, using
#  a text file which maps track number to desired channel.
 
# Usage: ./fix-channels.sh song.mid mapping.txt output.mid

# 1. Convert MIDI to CSV
./midicsv "$1" temp.csv

# 2. Build an AWK command from the mapping file
# We create a series of "if" statements for the AWK script
# This ignores blank lines or lines starting with #
map_logic=$(awk -F: '/^[0-9]/ {print "if ($1 == " $1 ") $4 = " ($2-1) ";"}' "$2")

# 3. Process the CSV using the dynamic logic
awk -F ', ' -v OFS=', ' "{
    if (\$3 ~ /_c$/ || \$3 == \"Pitch_bend_c\") {
        $map_logic
    }
    print \$0
}" temp.csv > fixed.csv

# 4. Convert back to MIDI
./csvmidi fixed.csv "$3"

# Clean up
rm temp.csv fixed.csv
echo "Processed $1 into $3 using $2"

