import sys
import mido
import re
from pathlib import Path
import shutil

ROLE_CHANNELS = {
    'player': 0,
    'piano-l': 1,
    'piano-r': 2,
    'piano-c': 3,
    'bass': 7,
    'percussion': 9
}

def to_camel_case(text):
    return "".join(word.capitalize() for word in re.split(r'[^a-zA-Z0-9]', text) if word)

def setup_paths(composer, piece):
    comp_camel = to_camel_case(composer)
    piece_camel = to_camel_case(piece)
    folder_name = f"{comp_camel}-{piece_camel}"
    target_dir = Path("Songs") / folder_name
    target_dir.mkdir(parents=True, exist_ok=True)
    
    file_name = f"{comp_camel.lower()}-{piece_camel.lower()}.mid"
    score_name = f"{comp_camel.lower()}-{piece_camel.lower()}-score.mid"
    
    return target_dir, target_dir / file_name, target_dir / score_name

def analyze_tracks(mid):
    print("\n🔍 Analyzing MIDI Tracks...")
    print("-" * 40)
    track_data = []
    for i, track in enumerate(mid.tracks):
        name = "Unnamed"
        notes = 0
        for msg in track:
            if msg.type == 'track_name': name = msg.name
            elif msg.type == 'note_on' and msg.velocity > 0: notes += 1
        track_data.append({'index': i, 'name': name, 'notes': notes})
        print(f"[{i}] {name: <20} | {notes} notes")
    return track_data

def get_user_mapping(track_data):
    print("-" * 40)
    mapping = {}
    player_idx = int(input("🎹 Enter track number for Player: "))
    mapping[player_idx] = ROLE_CHANNELS['player']

    print(f"\n🥁 Backing Roles: {', '.join([r for r in ROLE_CHANNELS if r != 'player'])}")
    while True:
        route = input("Route track (Track:role) or Enter to finish: ").strip().lower()
        if not route: break
        try:
            trk, role = route.split(':')
            mapping[int(trk)] = ROLE_CHANNELS[role.strip()]
        except: print("❌ Use format 2:bass")
    
    # NEW: Resolution prompt
    res = input("\n⏱️ Enter quantization resolution (8, 16, or 32 - default 16): ").strip()
    resolution = int(res) if res.isdigit() else 16
    
    return mapping, player_idx, resolution

def quantize_track(track, ticks_per_beat, resolution):
    """Snaps Note On/Off to grid and ensures a minimum duration."""
    grid_size = ticks_per_beat * (4.0 / resolution)
    min_duration = grid_size  # Ensure notes are at least one grid-unit long
    
    abs_time = 0
    events = []
    active_notes = {} # To track note_on/note_off pairs

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
            elif msg.type == 'note_off' or (msg.type == 'note_on' and msg.velocity == 0):
                on_time = active_notes.get(msg.note, new_abs - grid_size)
                # Ensure the note has at least 'min_duration' length
                event['abs_time'] = max(new_abs, on_time + min_duration)

    events.sort(key=lambda e: e['abs_time'])

    # 3. Rebuild Delta Times
    new_track = mido.MidiTrack()
    last_time = 0
    for event in events:
        delta = int(event['abs_time'] - last_time)
        new_track.append(event['msg'].copy(time=max(0, delta)))
        last_time = event['abs_time']
    return new_track

def process_audio_pipeline(input_path, output_path, mapping):
    mid = mido.MidiFile(input_path)
    new_mid = mido.MidiFile(ticks_per_beat=mid.ticks_per_beat, type=mid.type)
    keep = list(mapping.keys())
    if 0 not in keep: keep.append(0)

    for i, track in enumerate(mid.tracks):
        if i not in keep: continue
        new_track = mido.MidiTrack()
        new_mid.tracks.append(new_track)
        chan = mapping.get(i)
        for msg in track:
            if msg.type == 'control_change' and msg.control in (7, 10, 11): continue
            new_track.append(msg.copy(channel=chan) if hasattr(msg, 'channel') and chan is not None else msg.copy())
    new_mid.save(output_path)

def process_score_pipeline(input_path, output_path, player_idx, resolution):
    mid = mido.MidiFile(input_path)
    new_mid = mido.MidiFile(ticks_per_beat=mid.ticks_per_beat, type=mid.type)
    for i, track in enumerate(mid.tracks):
        if i not in (0, player_idx): continue
        if i == player_idx:
            q_track = quantize_track(track, mid.ticks_per_beat, resolution)
            new_mid.tracks.append(mido.MidiTrack([m.copy(channel=0) if hasattr(m, 'channel') else m for m in q_track]))
        else:
            new_mid.tracks.append(track.copy())
    new_mid.save(output_path)

def get_time_signature(mid):
    """Scans the MIDI file for a time signature meta-message."""
    for track in mid.tracks:
        for msg in track:
            if msg.type == 'time_signature':
                return msg.numerator, msg.denominator
    return None, None

def update_songs_database(composer, piece, target_dir, audio_path, num, den):
    """Appends a new song entry to the tab-separated songs.csv file."""
    db_path = Path("songs.csv")
    
    level = "1"
    new_round = 1
    
    # 1. Read the last row to get the previous Level and Round
    if db_path.exists():
        with open(db_path, 'r', encoding='utf-8') as f:
            lines = [line.strip() for line in f if line.strip()]
            if lines:
                last_row = lines[-1].split('\t')
                if len(last_row) >= 2:
                    level = last_row[0]
                    try:
                        new_round = int(last_row[1]) + 1
                    except ValueError:
                        pass
                        
    # 2. Get the Subtitle interactively
    print("\n" + "-" * 40)
    subtitle = input("📝 Enter the Subtitle (or leave blank if none): ").strip()
    
    # 3. Format the data
    internal_name = audio_path.stem
    loc_title = f"@@@{piece}"
    loc_subtitle = f"@@@{subtitle}" if subtitle else ""
    loc_composer = f"@@@{composer}"
    midi_file_path = f"{target_dir.name}/{audio_path.name}"
    
    # --- NEW: Count-in Columns ---
    # e.g., Songs/Count-in/count-in-4-4.mid
    count_in_midi = f"Songs/Count-in/count-in-{num}-{den}.mid"
    # e.g., 4
    count_in_beats = str(num)
    # e.g., count-in-4.txt
    count_in_gui = f"count-in-{num}.txt"
    
    # 4. Build the row array
    new_row = [
        level,
        str(new_round),
        internal_name,
        loc_title,
        loc_subtitle,
        loc_composer,
        midi_file_path,
        count_in_midi,
        count_in_beats,
        count_in_gui
    ]
    
    # 5. Append to the file
    with open(db_path, 'a', encoding='utf-8') as f:
        f.write("\t".join(new_row) + "\n")
        
    print(f"\n✅ Added '{internal_name}' to songs.csv (Level {level}, Round {new_round})")
    print(f"⏱️  Count-in set to: {num}/{den}")
    
    return new_row

def main():
    if len(sys.argv) < 2: sys.exit("Usage: python3 add_song.py <file.mid>")
    input_path = Path(sys.argv[1])
    mid = mido.MidiFile(input_path)
    
    track_data = analyze_tracks(mid)
    mapping, player_idx, resolution = get_user_mapping(track_data)
    
    piece = input("\n📝 Piece Name: ")
    composer = input("📝 Composer: ")

    target_dir, audio_path, score_path = setup_paths(composer, piece)
    shutil.copy(input_path, target_dir / "original_backup.mid")

    process_audio_pipeline(input_path, audio_path, mapping)
    process_score_pipeline(input_path, score_path, player_idx, resolution)

    # --- Get Time Signature - extract from midi data if poss.  ---
    num, den = get_time_signature(mid)
    if num is not None and den is not None:
        print(f"\n⏱️ Detected Time Signature: {num}/{den}")
    else:
        print("\n⚠️ No time signature found in the MIDI file.")
        while True:
            ts_input = input("⏱️ Enter the time signature (e.g., 4/4, 3/4, 6/4): ").strip()
            try:
                num_str, den_str = ts_input.split('/')
                num, den = int(num_str), int(den_str)
                break
            except ValueError:
                print("❌ Invalid format. Please use numerator/denominator (e.g., 4/4).")

    with open(target_dir / "song_meta.txt", 'w') as f:
        f.write(f"Composer={composer}\nPiece={piece}\nQuantization={resolution}\n")

    # --- Update the game database ---
    # Pass num and den into the database function
    update_songs_database(composer, piece, target_dir, audio_path, num, den)
    
    print(f"\n🎉 Finished! Files ready in {target_dir}")

if __name__ == "__main__":
    main()

