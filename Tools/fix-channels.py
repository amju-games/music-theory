# fix-channels.py
# Strip out unwanted midi control events, which interfere with panning etc.
# Using the 'channels.txt' file, map all events in tracks to desired channels.
#
# To install mido, the recommendation is to create a virtual env and install it 
#  there.
# On mac:
#   python3 -m venv .venv       # create virtual environment
#   source .venv/bin/activate   # activate it
#   pip3 install mido           # install mido library
#
# Or just:
#   pip3 install mido 
# to install system-wide.

import sys
import mido

def process_midi(input_file, mapping_file, output_file):
    # 1. Read the mapping file
    mapping = {}
    try:
        with open(mapping_file, 'r') as f:
            for line in f:
                # Ignore comments and blank lines
                line = line.split('#')[0].strip()
                if not line:
                    continue
                
                parts = line.split(':')
                if len(parts) == 2:
                    track_idx = int(parts[0].strip())
                    # Convert channel from 1-16 (text file) to 0-15 (MIDI standard)
                    channel_val = int(parts[1].strip()) - 1 
                    mapping[track_idx] = channel_val
    except FileNotFoundError:
        print(f"Error: Could not find mapping file '{mapping_file}'")
        sys.exit(1)

    # 2. Open the original MIDI file
    try:
        mid = mido.MidiFile(input_file)
    except Exception as e:
        print(f"Error loading MIDI file: {e}")
        sys.exit(1)
        
    # Create a new blank MIDI file with the same timing resolution and type
    new_mid = mido.MidiFile(ticks_per_beat=mid.ticks_per_beat, type=mid.type)

    # 3. Process Tracks and Messages
    for current_track_idx, track in enumerate(mid.tracks):
        new_track = mido.MidiTrack()
        new_mid.tracks.append(new_track)
        
        # midicsv uses 1-based track numbering. We add 1 so your old txt files still work.
        track_lookup = current_track_idx + 1 
        target_channel = mapping.get(track_lookup)

        for msg in track:
            # --- FEATURE 1: Strip Problematic CCs ---
            if msg.type == 'control_change' and msg.control in (7, 10, 11):
                continue # Skip this message entirely!

            # --- FEATURE 2: Apply Channel Mapping ---
            # Mido messages are immutable, so we create a modified copy
            if target_channel is not None and hasattr(msg, 'channel'):
                new_msg = msg.copy(channel=target_channel)
            else:
                new_msg = msg.copy() # Keep it as is
                
            new_track.append(new_msg)

    # 4. Save the result
    new_mid.save(output_file)
    print(f"Successfully processed '{input_file}' -> '{output_file}'")
    print(f"(Channels remapped via {mapping_file}, CC 7/10/11 stripped)")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python fix_channels.py input.mid mapping.txt output.mid")
        sys.exit(1)
        
    process_midi(sys.argv[1], sys.argv[2], sys.argv[3])

