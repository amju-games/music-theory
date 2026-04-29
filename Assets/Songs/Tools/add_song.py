import sys
import mido
import re
from pathlib import Path
import shutil
import subprocess

def generate_score_files(comp_camel, piece_camel, target_dir, score_midi_path, num, den, resolution, bpm):
    """
    Step 1: Runs ./midiscore to create .makescore.txt
    Step 2: Runs ./makescore to create .score.txt
    """
    base_name = f"{comp_camel.lower()}-{piece_camel.lower()}"
    makescore_path = target_dir / f"{base_name}.makescore.txt"
    final_score_path = target_dir / f"{base_name}.score.txt"
    
    # --- STEP 1: midiscore ---
    cmd1 = [
        "./midiscore",
        str(score_midi_path),
        "--timesig", f"{num}/{den}",
        "--quant", resolution,
        "--bpm", str(bpm)
    ]
    
    print(f"🎼 Step 1: Running midiscore...")
    try:
        with open(makescore_path, "w") as f:
            subprocess.run(cmd1, stdout=f, check=True)
    except subprocess.CalledProcessError as e:
        print(f"❌ midiscore failed: {e}")
        return None

    # --- STEP 2: makescore ---
    # Note: We pass the path to the .makescore.txt we just created
    cmd2 = [
        "./makescore",
        "--file", str(makescore_path)
    ]
    
    print(f"🎼 Step 2: Running makescore...")
    try:
        # Assuming makescore writes the .score.txt file itself or to stdout
        # If it writes to stdout, we redirect it:
        with open(final_score_path, "w") as f:
            subprocess.run(cmd2, stdout=f, check=True)
        print(f"✅ Created: {final_score_path.name}")
    except subprocess.CalledProcessError as e:
        print(f"❌ makescore failed: {e}")
        return None

    # Return the string formatted for the CSV column: 'Composer-Piece/composer-piece.score.txt'
    return f"{target_dir.name}/{final_score_path.name}"


ROLE_CHANNELS = {
    'player': 0,
    'piano-l': 1,
    'piano-r': 2,
    'piano-c': 3,
    'bass': 7,
    'percussion': 9
}

# Maps your shorthand to (Numerical Resolution, Midiscore Flag)
QUANT_MAP = {
    "c": (4, "c"),      # Crotchet (1/4)
    "q": (8, "q"),      # Quaver (1/8)
    "qq": (16, "qq"),    # Semiquaver (1/16)
    "qqq": (32, "qqq")   # Demisemiquaver (1/32)
}

def get_quantization_setting():
    """Prompts the user for a musical quantization code."""
    print("\n⏱️  Quantization Settings:")
    print("   c   = Crotchet     (1/4)")
    print("   q   = Quaver       (1/8)")
    print("   qq  = Semiquaver   (1/16)")
    print("   qqq = Demisemiquaver (1/32)")
    
    while True:
        q_code = input("\nEnter resolution code [qq]: ").strip().lower()
        if not q_code: 
            q_code = "qq" # Default to Semiquaver
        
        if q_code in QUANT_MAP:
            num_res, flag = QUANT_MAP[q_code]
            print(f"   ✅ Set to {q_code.upper()}")
            return num_res, flag
        else:
            print("   ❌ Invalid code. Please use c, q, qq, or qqq.")

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
    
    return mapping, player_idx

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


def update_songs_database(composer, piece, target_dir, audio_path, num, den, score_path_string):
    """Appends the new columns to songs.csv."""
    db_path = Path("songs.csv")
    
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
    
    print(f"\n✅ Added '{internal_name}' to songs.csv (Level {level}, Round {new_round})")
    print(f"⏱️  Count-in set to: {num}/{den}")
    
    # Columns so far:
    # 0:Level, 1:Round, 2:Internal, 3:Title, 4:Subtitle, 5:Composer, 6:MidiPath
    # 7:CountInMidi, 8:CountInBeats, 9:CountInGui
    
    # 10. Palette
    palette = "palette-notes-12-1.png"
    # 11. Score Filename
    score_file = score_path_string
    
    new_row = [
        level, str(new_round), internal_name, loc_title, loc_subtitle, loc_composer,
        midi_file_path, count_in_midi, count_in_beats, count_in_gui,
        palette, score_file
    ]
    
    with open(db_path, 'a', encoding='utf-8') as f:
        f.write("\t".join(new_row) + "\n")
        
    print(f"📊 Database updated with score path: {score_file}")

    
    level = "1"
    return new_row


def get_or_prompt_time_signature(mid):
    """Scans for a time signature meta-message, or asks the user if missing."""
    # Try to detect automatically first
    for track in mid.tracks:
        for msg in track:
            if msg.type == 'time_signature':
                print(f"\n⏱️ Detected Time Signature: {msg.numerator}/{msg.denominator}")
                return msg.numerator, msg.denominator
                
    # Fallback if not found
    print("\n⚠️ No time signature found in the MIDI file.")
    while True:
        ts_input = input("⏱️ Enter the time signature (e.g., 4/4, 3/4, 6/4): ").strip()
        try:
            num_str, den_str = ts_input.split('/')
            num, den = int(num_str), int(den_str)
            return num, den
        except ValueError:
            print("❌ Invalid format. Please use numerator/denominator (e.g., 4/4).")

def get_or_prompt_bpm(mid):
    """Scans for a tempo meta-message, converts to BPM, and asks user to confirm."""
    extracted_bpm = None
    
    # Try to detect automatically first
    for track in mid.tracks:
        for msg in track:
            if msg.type == 'set_tempo':
                # mido.tempo2bpm returns a float, so we round it to a clean integer
                extracted_bpm = round(mido.tempo2bpm(msg.tempo))
                break 
        if extracted_bpm:
            break

    # Set our default display string
    default_bpm_str = str(extracted_bpm) if extracted_bpm else "120"
    
    if extracted_bpm:
        print(f"\n🎵 Detected Tempo: {extracted_bpm} BPM")
    else:
        print("\n⚠️ No tempo found in the MIDI file.")
        
    while True:
        bpm_input = input(f"⏱️ Enter the BPM [{default_bpm_str}]: ").strip()
        
        # If the user just hits Enter, use the default
        if not bpm_input:
            return default_bpm_str
            
        if bpm_input.isdigit():
            return bpm_input
        else:
            print("❌ Please enter a valid number for BPM.")


def main():
    # 1. Setup and Validation
    if len(sys.argv) < 2:
        print("Usage: python3 add_song.py <input.mid>")
        sys.exit(1)

    input_path = Path(sys.argv[1])
    if not input_path.exists():
        print(f"❌ File not found: {input_path}")
        sys.exit(1)

    print(f"🎵 Initializing One-Minute Importer for: {input_path.name}")
    mid = mido.MidiFile(input_path)
    
    # 2. Track Analysis and Routing
    track_data = analyze_tracks(mid)
    mapping, player_idx = get_user_mapping(track_data)
    num_res, q_flag = get_quantization_setting()

    # 3. Time Signature and Tempo
    num, den = get_or_prompt_time_signature(mid)
    bpm = get_or_prompt_bpm(mid)
    
    # 4. Metadata Gathering
    piece_name = input("\n📝 Enter the name of the Piece: ")
    composer_name = input("📝 Enter the name of the Composer: ")

    comp_camel = to_camel_case(composer_name)
    piece_camel = to_camel_case(piece_name)

    # 5. File System Setup
    target_dir, audio_path, score_midi_path = setup_paths(composer_name, piece_name)
    print(f"\n📁 Created directory: {target_dir}")
    shutil.copy(input_path, target_dir / "original_backup.mid")

    # 6. MIDI Pipeline Processing
    print(f"⚙️ Generating Audio File: {audio_path.name}...")
    process_audio_pipeline(input_path, audio_path, mapping)
    
    print(f"🎼 Generating UI Score File: {score_midi_path.name}...")
    process_score_pipeline(input_path, score_midi_path, player_idx, num_res)
    
    # 7. Local Metadata Text File
    with open(target_dir / "song_meta.txt", 'w') as f:
        f.write(f"Composer={composer_name}\n")
        f.write(f"Piece={piece_name}\n")
        f.write(f"Quantization={num_res}\n")
        f.write(f"PlayerChannel={ROLE_CHANNELS['player']}\n")
        
    # 8. Execute External Score Binaries (midiscore & makescore)
    score_csv_string = generate_score_files(
        comp_camel, piece_camel, target_dir, score_midi_path, num, den, q_flag, bpm
    )
    
    # 9. Update the Game Database
    if score_csv_string:
        update_songs_database(
            composer_name, piece_name, target_dir, audio_path, num, den, score_csv_string
        )
    else:
        print("\n⚠️ Database update skipped due to score generation failure.")
        
    print(f"\n🎉 Finished! Song successfully added to the game pipeline.")

if __name__ == "__main__":
    main()


