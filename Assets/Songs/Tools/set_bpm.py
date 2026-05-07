import sys
import mido
from pathlib import Path

def set_midi_bpm(input_file, new_bpm):
    # Load the MIDI file
    try:
        mid = mido.MidiFile(input_file)
    except Exception as e:
        print(f"❌ Error loading file: {e}")
        return

    # Convert BPM to MIDI tempo (microseconds per beat)
    new_tempo = mido.bpm2tempo(new_bpm)
    
    # Process tracks
    for i, track in enumerate(mid.tracks):
        # Remove all existing tempo messages from this track
        # We use a list comprehension to filter out 'set_tempo'
        mid.tracks[i] = mido.MidiTrack([msg for msg in track if msg.type != 'set_tempo'])

    # Inject the new tempo at the start of Track 0
    # Note: time=0 ensures it happens at the very beginning
    tempo_msg = mido.MetaMessage('set_tempo', tempo=new_tempo, time=0)
    mid.tracks[0].insert(0, tempo_msg)

    # Generate output filename
    output_path = Path(input_file).stem + f"_{new_bpm}bpm.mid"
    mid.save(output_path)
    print(f"✅ Successfully set BPM to {new_bpm}")
    print(f"💾 Saved as: {output_path}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python set_bpm.py <file.mid> <bpm>")
        print("Example: python set_bpm.py song.mid 120")
        sys.exit(1)

    file_arg = sys.argv[1]
    try:
        bpm_arg = float(sys.argv[2])
        set_midi_bpm(file_arg, bpm_arg)
    except ValueError:
        print("❌ Error: BPM must be a number.")


