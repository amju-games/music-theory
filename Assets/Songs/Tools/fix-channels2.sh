#!/bin/bash

# MIDI files exported from Studio One sadly do not respect channel
# mappings, with all channels set to zero. 
# This script uses midicsv to fix the channels for each track, using
# a text file which maps track number to desired channel.
# It also strips out CC 7 (Volume), 10 (Pan), and 11 (Expression) 
# so BASS code has total control.

# Usage: ./fix-channels.sh song.mid mapping.txt output.mid

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 input.mid mapping.txt output.mid"
    exit 1
fi

# 1. Convert MIDI to CSV
./midicsv "$1" temp.csv

# 2. Build an AWK command from the mapping file
# Maps track number to desired channel (0-indexed)
map_logic=$(awk -F: '/^[0-9]/ {print "if ($1 == " $1 ") $4 = " ($2-1) ";"}' "$2")

# 3. Process the CSV: Fix channels AND strip problematic controllers
# We skip (delete) lines that are Control_c for 7, 10, or 11
awk -F ', ' -v OFS=', ' "{
    # First: Strip CC 7 (Vol), 10 (Pan), 11 (Expression)
    if (\$3 == \"Control_c\") {
        if (\$5 == 7 || \$5 == 10 || \$5 == 11) {
            next; # Delete line and move to next record
        }
    }

    # Second: Apply Channel Mapping
    if (\$3 ~ /_c$/ || \$3 == \"Pitch_bend_c\") {
        $map_logic
    }

    print \$0
}" temp.csv > fixed.csv

# 4. Convert back to MIDI
./csvmidi fixed.csv "$3"

# Clean up
rm temp.csv 
#rm fixed.csv
echo "Processed $1 into $3 using $2 (Channels remapped, CC 7/10/11 stripped)"

