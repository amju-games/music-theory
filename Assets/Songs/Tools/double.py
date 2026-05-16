# Double note values (c -> m, q -> c, etc), maintaining the same tempo.

import sys
import os
import mido
from mido import MidiFile

import re
from pathlib import Path

# Get a new filename: add _x2 if no multiplier suffix.
# If a suffix already exists, double it.
# E.g.
#   update_filename("image.png") ->  image_x2.png
#   update_filename("image_x2.png") ->  image_x4.png
#
def update_filename(filename: str) -> str:
    path = Path(filename)
    stem = path.stem  # Filename without extension (e.g., "image_x2")
    ext = path.suffix  # File extension (e.g., ".png")

    # Match '_x' followed by any digits at the very end of the stem
    match = re.search(r'_x(\d+)$', stem)

    if match:
        # Extract the current number, multiply it by 2
        current_multiplier = int(match.group(1))
        new_multiplier = current_multiplier * 2
        # Replace the old suffix with the new one
        new_stem = re.sub(r'_x\d+$', f'_x{new_multiplier}', stem)
    else:
        # No existing suffix, append the base '_x2'
        new_stem = f"{stem}_x2"

    return f"{new_stem}{ext}"


def double_notes_and_maintain_speed(input_path, output_path):
    mid = MidiFile(input_path)
    
    for track in mid.tracks:
        for msg in track:
            # Step 1: Stretch out the placement grid for all standard messages
            msg.time = msg.time * 2
            
            # Step 2: Intercept meta tempo messages to fix the speed drop
            if msg.is_meta and msg.type == 'set_tempo':
                # Halving the microseconds per beat doubles the playback speed
                msg.tempo = int(round(msg.tempo / 2))
                
    mid.save(output_path)
    print(f"✅ Done! New file with note values doubled: {output_path}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Double note values (c -> m, q -> c, etc.), maintaining the same tempo.")
        print("Usage: python3 double.py <file.mid>")
        print("Example: `python3 double.py song.mid` will generate song_x2.mid")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = update_filename(input_path)
    double_notes_and_maintain_speed(input_path, output_path)

