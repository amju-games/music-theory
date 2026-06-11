# add_bass_track.py
# Add a bass track by taking the low notes from another track and
#  assigning the events to the bass channel.
# See add_song.py for how to install mido

import sys  
import mido
from pathlib import Path
from music_utils import note_to_midi
from collections import defaultdict

def deduplicate_bass_track(bass_track):
    """
    Parses a MIDI track, pairs note_on/offs, takes the mathematical union 
    of all overlapping intervals of the same pitch, and reconstructs the track.
    """
    # 1. Convert relative delta times to absolute timelines
    abs_time = 0
    pending_notes = {}   # key: pitch, value: (start_time, velocity)
    raw_notes = []       # list of dicts: {pitch, start, end, velocity}
    meta_messages = []   # Save track names, tempos, text, etc.

    for msg in bass_track:
        abs_time += msg.time
        if msg.is_meta:
            meta_messages.append((abs_time, msg.copy()))
            continue
            
        if msg.type == 'note_on' and msg.velocity > 0:
            # If the same pitch is already playing, close it out first to capture it
            if msg.note in pending_notes:
                start_t, vel = pending_notes[msg.note]
                raw_notes.append({'pitch': msg.note, 'start': start_t, 'end': abs_time, 'velocity': vel})
            pending_notes[msg.note] = (abs_time, msg.velocity)
            
        elif msg.type in ('note_off', 'note_on'): # note_on with vel 0 is a note_off
            if msg.note in pending_notes:
                start_t, vel = pending_notes.pop(msg.note)
                raw_notes.append({'pitch': msg.note, 'start': start_t, 'end': abs_time, 'velocity': vel})

    # Close out any remaining dangling notes at the very end of the performance
    for pitch, (start_t, vel) in pending_notes.items():
        raw_notes.append({'pitch': pitch, 'start': start_t, 'end': abs_time, 'velocity': vel})

    # --- RECORD INITIAL NOTE COUNT ---
    initial_note_count = len(raw_notes)

    # 2. STRATEGY: Group by pitch, sort by start time, and merge overlapping intervals (Union)
    notes_by_pitch = defaultdict(list)
    for note in raw_notes:
        notes_by_pitch[note['pitch']].append(note)

    unionized_notes = []

    for pitch, note_list in notes_by_pitch.items():
        # Sort intervals chronologically by their start times
        note_list.sort(key=lambda x: x['start'])
        
        current_merged = note_list[0]
        
        for next_note in note_list[1:]:
            # If the next note starts before (or exactly when) the current one ends, they overlap!
            if next_note['start'] <= current_merged['end']:
                # Union the boundaries: extend the end time to whichever is further down the line
                current_merged['end'] = max(current_merged['end'], next_note['end'])
                # Keep the loudest velocity accent from the overlapping cluster
                current_merged['velocity'] = max(current_merged['velocity'], next_note['velocity'])
            else:
                # No overlap! Save the completed interval and move to the next cluster link
                unionized_notes.append(current_merged)
                current_merged = next_note
                
        # Append the final remaining note cluster for this pitch
        unionized_notes.append(current_merged)

    # --- RECORD FINAL NOTE COUNT ---
    final_note_count = len(unionized_notes)
    removed_notes = initial_note_count - final_note_count

    # 3. Reconstruct raw MIDI timeline events
    timeline_events = []
    
    # Put track meta headers back safely
    for abs_t, meta_msg in meta_messages:
        timeline_events.append((abs_t, meta_msg))
        
    # Split our merged objects back into explicit MIDI on/off commands
    for note in unionized_notes:
        on = mido.Message('note_on', note=note['pitch'], velocity=note['velocity'], channel=8)
        off = mido.Message('note_off', note=note['pitch'], velocity=0, channel=8)
        timeline_events.append((note['start'], on))
        timeline_events.append((note['end'], off))

    # Sort everything chronologically across the shared track timeline
    timeline_events.sort(key=lambda x: x[0])

    # 4. Rebuild the original bass_track with clean relative delta times
    bass_track.clear()
    last_time = 0
    for abs_t, msg in timeline_events:
        msg.time = abs_t - last_time
        bass_track.append(msg)
        last_time = abs_t

    # --- 📊 PRINT THE REPORT ---
    print(f"\n  🗜️  Bass Line Optimization Report:")
    print(f"     • Original Notes Gathered: {initial_note_count}")
    print(f"     • Final Cleaned Notes:     {final_note_count}")
    if removed_notes > 0:
        print(f"     • Overlapping Duplicates Removed/Merged: ✨ {removed_notes} ✨")
    else:
        print(f"     • No overlapping notes detected; track is pristine!")


def get_add_bass_track():
    yes_bass = input("🍔 Would you like me to add an auto-generated bass track? (y/n): ").strip().lower()
    return (yes_bass == 'y')


def copy_notes_to_bass_track(mid, bass_track):
    while True: 
        # Get track index from user
        track_index_str = input("🍟 Enter a track number to use, or Enter to finish: ")
        if not track_index_str: break
        track_index = int(track_index_str)
 
        # Get the chosen track 
        try:
            track = mid.tracks[track_index]
        except:
            print("  ❌ Couldn't get that track!")
            continue

        # Get top bass note from user
        default_top_bass_note = 48
        while True:
            top_bass_str = input(f"🎵 Enter highest note we will extract to the bass line [C3 / 48]: ").strip()
            
            if not top_bass_str: 
                top_bass_note = default_top_bass_note
                print(f"   Using default: {top_bass_note}")
                break
                
            try:
                top_bass_note = note_to_midi(top_bass_str)
                print(f"   Mapped '{top_bass_str}' to MIDI Note: {top_bass_note}")
                break
            except ValueError as e:
                print(f"   ❌ {e} Please try again.")

        # Copy notes in the chosen track which are classed as bass notes
        num_events = 0 # count note on and note off events
        for msg in track:
            if (msg.type == 'note_on' or msg.type == 'note_off') and msg.note <= top_bass_note:
                msg_copy = msg.copy()
                msg_copy.channel = 8 # set bass channel
                bass_track.append(msg_copy)
                num_events += 1

        # Report how that went
        if num_events == 0:
            print(f"  ⚠️  No notes copied. Try with a higher pitch threshold or a different track.")
        else:
            # Clean up any duplicated or overlapping notes
            deduplicate_bass_track(bass_track)
            print(f"  ✅ Done! {int(num_events / 2)} notes ({num_events} events) copied from {track.name} to {bass_track.name}.") 
            

def show_info(mid):
    print("\n🥩 MIDI file info:")
    for i, track in enumerate(mid.tracks):
        name = "Unnamed"
        max_pitch = 0
        min_pitch = 200
        num_notes = 0
        for msg in track:
            if msg.type == 'track_name': name = msg.name
            elif msg.type == 'note_on' and msg.velocity > 0: 
                max_pitch = max(max_pitch, msg.note)
                min_pitch = min(min_pitch, msg.note)
                num_notes += 1
        if (num_notes > 0):
            print(f"[{i}] {name: <20} | {num_notes} notes, pitch range {min_pitch} - {max_pitch}")
        else:
            print(f"[{i}] {name: <20} | No notes.")


def create_new_midi_with_bass_track(input_filename):
    # Open input file
    try:
        mid = mido.MidiFile(input_filename)
    except Exception as e:
        print(f"Error loading MIDI file: {e}")
        sys.exit(1) 

    # Show info for input midi file
    show_info(mid)

    # Create a new midi file, copy tracks from input 
    new_mid = mido.MidiFile(ticks_per_beat=mid.ticks_per_beat, type=mid.type)
    for i, track in enumerate(mid.tracks):
        new_track = mido.MidiTrack()
        new_mid.tracks.append(new_track)
        for msg in track:
            new_track.append(msg.copy())

    # Add new, empty bass track
    new_track = mido.MidiTrack()
    new_mid.tracks.append(new_track)
    # Insert track name meta message at time = 0
    new_track_name = "Bass (auto generated)"
    new_track.append(mido.MetaMessage('track_name', name=new_track_name, time=0))
    return new_mid, new_track


def make_output_filename(filename: input_filename) -> str:
    path = Path(filename)
    stem = path.stem
    ext = path.suffix
    return stem + "_with_bass" + ext;


def add_bass_track_if_required():
    if (get_add_bass_track()):
        input_filename = sys.argv[1];
        # Create new file with empty bass track
        new_mid, new_track = create_new_midi_with_bass_track(input_filename)
        # Add notes from other tracks, using a cut off highest note
        copy_notes_to_bass_track(new_mid, new_track)
        # Save new midi file
        output_filename = make_output_filename(input_filename)
        new_mid.save(output_filename)
        # Report new filename
        print(f"🎉 Finished! New midi file with bass track: {output_filename}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 add_bass_track input.mid")
        sys.exit(1)
    add_bass_track_if_required()

