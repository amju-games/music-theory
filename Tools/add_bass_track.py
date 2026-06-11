# add_bass_track.py
# Add a bass track by taking the low notes from another track and
#  assigning the events to the bass channel.
# See add_song.py for how to install mido

import sys  
import mido
from pathlib import Path
from music_utils import note_to_midi

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

