# quantise.py
# Helper functions for add_song.py

import sys
import mido
import re
from pathlib import Path
import shutil
import subprocess
import platform

# Maps your shorthand to (Numerical Resolution, Midiscore Flag)
QUANT_MAP = {
    "c": (4, "c"),      # Crotchet (1/4)
    "q": (8, "q"),      # Quaver (1/8)
    "qq": (16, "qq"),    # Semiquaver (1/16)
    "qqq": (32, "qqq")   # Demisemiquaver (1/32)
}

def detect_suggested_quantization(player_track, ticks_per_beat):
    """Scans the track for the shortest gap between notes to suggest a grid size."""
    abs_time = 0
    onsets = []
    
    for msg in player_track:
        abs_time += msg.time
        if msg.type == 'note_on' and msg.velocity > 0:
            onsets.append(abs_time)
            
    if len(onsets) < 2:
        return "qq", False # Default with no warning
        
    # We ignore gaps smaller than a 128th note (ticks_per_beat / 32). 
    # Anything faster than that is almost certainly a human playing a chord.
    slop_threshold = ticks_per_beat / 32 
    
    intervals = []
    for i in range(1, len(onsets)):
        diff = onsets[i] - onsets[i-1]
        if diff > slop_threshold:
            intervals.append(diff)
            
    if not intervals:
        return "qq", False
        
    min_interval = min(intervals)
    
    # --- Resolution Check ---
    # A 32nd note is exactly ticks_per_beat / 8. 
    # If the gap is smaller than this, 'qqq' won't be enough!
    resolution_warning = min_interval < (ticks_per_beat / 8)
    
    # Match to the grid
    if min_interval <= ticks_per_beat / 6: 
        return "qqq", resolution_warning
    elif min_interval <= ticks_per_beat / 3:
        return "qq", resolution_warning
    elif min_interval <= ticks_per_beat / 1.5:
        return "q", resolution_warning
    else:
        return "c", resolution_warning

def get_quantization_setting(player_track, ticks_per_beat):
    """Prompts the user, auto-detects best values, displaying a warning if runs are too fast for the grid."""
    
    # Unpack the new warning flag
    suggested_code, warning = detect_suggested_quantization(player_track, ticks_per_beat)

    print("\n⏱️  Quantization Settings:")
    print("   c   = Crotchet     (1/4)")
    print("   q   = Quaver       (1/8)")
    print("   qq  = Semiquaver   (1/16)")
    print("   qqq = Demisemiquaver (1/32)")
    print(f"   💡 Auto-detected suggestion: {suggested_code.upper()}")
   
    # --- Display the warning ---
    if warning:
        print("   ⚠️  WARNING: Detected notes faster than 1/32nd resolution!")
        print("       These fast notes/trills will likely render as chords.")
 
    while True:
        # Inject our dynamic suggestion into the input prompt
        q_code = input(f"\nEnter resolution code [{suggested_code}]: ").strip().lower()
        
        if not q_code: 
            q_code = suggested_code # Use the dynamic default
        
        if q_code in QUANT_MAP:
            num_res, flag = QUANT_MAP[q_code]
            print(f"   ✅ Set to {q_code.upper()}")
            return num_res, flag
        else:
            print("   ❌ Invalid code. Please use c, q, qq, or qqq.")


def quantize_track(track, ticks_per_beat, resolution):
    """Snaps Note On/Off to grid and ensures a minimum duration."""
    grid_size = ticks_per_beat * (4.0 / resolution)
    min_duration = grid_size  # Ensure notes are at least one grid-unit long
    
    abs_time = 0
    events = []
    active_notes = {} # To track note_on/note_off pairs
    # Chop durations of overlapping notes:
    most_recent_note = 0 # Most recent note, so we can detect overlaps
    most_recent_on_time = 0 # Abs time of most recent note_on event
    chopped_to_zero = 0 # Count notes chopped down to zero length

    # 1. Convert to Absolute Time
    for msg in track:
        abs_time += msg.time
        events.append({'msg': msg, 'abs_time': abs_time})

    # 2. Snap to Grid
    for event in events:
        msg = event['msg']
        if msg.type in ('note_on', 'note_off'):
            original_abs = event['abs_time']
            new_abs = round(original_abs / grid_size) * grid_size
            
            # Logic to prevent Note Off from moving BEFORE or ON TOP OF Note On
            if msg.type == 'note_on' and msg.velocity > 0:
                active_notes[msg.note] = new_abs
                event['abs_time'] = new_abs
                most_recent_note = msg.note
                most_recent_on_time = new_abs
            elif msg.note == most_recent_note:
                # It's a note off event, and we haven't been overlapped
                on_time = active_notes.get(msg.note, new_abs - grid_size)
                # Ensure the note has at least 'min_duration' length
                event['abs_time'] = max(new_abs, on_time + min_duration)
            else:
                # Note off event, with a more recent note overlapping -- chop!
                event['abs_time'] = most_recent_on_time
                if active_notes[msg.note] == most_recent_on_time: 
                    chopped_to_zero += 1

    if chopped_to_zero > 0:
        print(f"⚠️  WARNING: Quantisation chopped {chopped_to_zero} notes to zero length!")

    events.sort(key=lambda e: e['abs_time'])

    # 3. Rebuild Delta Times
    new_track = mido.MidiTrack()
    last_time = 0
    for event in events:
        delta = int(event['abs_time'] - last_time)
        new_track.append(event['msg'].copy(time=max(0, delta)))
        last_time = event['abs_time']
    return new_track

