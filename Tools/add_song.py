# add_song.py
# Add a new song to the game. 
# Usage example:
#    python3 add_song.py ~/Downloads/newsong.mid
#
#
# To install mido, the recommendation is to create a virtual env and install it 
#  there.
# On mac:
#   python3 -m venv .venv       # create virtual environment
#   source .venv/bin/activate   # activate it
#   pip3 install mido           # install mido library
#
# In each new shell, you need to activate the venv again:
#   source .venv/bin/activate   
#
# Or just:
#   pip3 install mido 
# to install system-wide.

import mido
from pathlib import Path
import platform
import re
import shutil
import subprocess
import sys
from quantise import *
from add_bass_track import add_bass_track_if_required_in_mem
from add_percussion_track import add_percussion_track_if_required_in_mem
from extract_top_line import extract_top_line_in_mem
from set_bpm import set_midi_bpm_in_mem

# Subdir to put workspace files: don't commit them, and don't add them 
#  to the game!
WORKSPACE = "workspace"

def get_exec_path(base_name):
    """Adjusts executable path based on the Operating System."""
    system = platform.system() # Returns 'Windows', 'Darwin' (Mac), or 'Linux'
    
    if system == "Windows":
        # Windows doesn't need ./ and expects .exe (though .exe is often optional)
        return f"{base_name}.exe" 
    else:
        # Mac and Linux require the relative path prefix
        return f"./{base_name}"

def generate_score_files(comp_camel, piece_camel, target_dir, workspace_dir, score_midi_path, num, den, resolution, bpm, new_player_idx):
    """
    Step 1: Runs ./midiscore to create .makescore.txt
    Step 2: Runs ./makescore to create .score.txt
    """
    base_name = f"{comp_camel.lower()}-{piece_camel.lower()}"
    makescore_path = workspace_dir / f"{base_name}.makescore.txt"
    final_score_path = target_dir / f"{base_name}.score.txt"

    # Get the correct executable names for the current OS
    midiscore_exec = get_exec_path("midiscore")
    makescore_exec = get_exec_path("makescore")

    # Ask if we should add an anacrusis, (an incomplete first bar)
    anacrusis = input("📝 Enter anacrusis length (q, c, m, etc., or leave blank if none): ").strip()

    # --- STEP 1: midiscore ---
    cmd1 = [
        midiscore_exec,
        str(score_midi_path),
        "--timesig", f"{num}/{den}",
        "--quant", resolution,
        "--bpm", str(bpm),
        "--track", str(new_player_idx - 1) # zero-based player track number
    ]
    if (anacrusis):
        cmd1.extend(["--ana", anacrusis])
    
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
        makescore_exec,
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

def to_camel_case(text):
    return "".join(word.capitalize() for word in re.split(r'[^a-zA-Z0-9]', text) if word)

def setup_paths(composer, piece):
    comp_camel = to_camel_case(composer)
    piece_camel = to_camel_case(piece)
    folder_name = f"{comp_camel}-{piece_camel}"
    target_dir = Path("../Assets/Songs") / folder_name
    target_dir.mkdir(parents=True, exist_ok=True)
    # Create workspace dir
    workspace_dir = target_dir / Path(WORKSPACE)
    workspace_dir.mkdir(parents=True, exist_ok=True)
   
    # Final midi file: correct channels, chosen tracks, extra events stripped. 
    file_name = f"{comp_camel.lower()}-{piece_camel.lower()}.mid"
    # Midi file with quantised player part for midiscore
    score_name = f"{comp_camel.lower()}-{piece_camel.lower()}-score.mid"
    
    return target_dir, target_dir / file_name, workspace_dir / score_name, workspace_dir

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


# Output new midi file with remapped channels, and tracks renamed to
#  reflect the mapping.
def process_audio_pipeline(mid, output_path, mapping):

    # the track->channel mapping after we chop unwanted tracks
    new_mapping = {} 

    # Create a reverse map for naming: {0: 'Player', 7: 'Bass', ...}
    # We capitalize the keys (Piano-C, etc.)
    chan_to_name = {v: k.replace('-', ' ').title().replace(' ', '-') for k, v in ROLE_CHANNELS.items()}
    
    # Track name counters to handle duplicates
    name_counters = {}

    new_mid = mido.MidiFile(ticks_per_beat=mid.ticks_per_beat, type=mid.type)
    keep = list(mapping.keys())
    if 0 not in keep: keep.append(0)

    # New track numbers: starting at 1 because these are 1-based. 
    #  Track 0 (=> 1, sigh) is the control track 
    track_num_1based = 1 

    for i, track in enumerate(mid.tracks):
        if i not in keep: continue
        new_track = mido.MidiTrack()
        new_mid.tracks.append(new_track)
        chan = mapping.get(i)

        # Store track num -> channel mapping
        if chan is not None:
            new_mapping[track_num_1based] = chan
        track_num_1based += 1

        # Rename track
        if chan is not None:
            base_name = chan_to_name.get(chan, f"Channel-{chan}")
    
            # Increment counter for this name
            name_counters[base_name] = name_counters.get(base_name, 0) + 1 
    
            # Append number if it's a duplicate
            final_name = base_name
            if name_counters[base_name] > 1:
                final_name = f"{base_name} {name_counters[base_name]}"
    
            # Insert the track name meta message at the very beginning
            new_track.append(mido.MetaMessage('track_name', name=final_name, time=0))

        # Accumulator for the delta time of skipped messages
        skipped_time = 0 
        
        for msg in track:
            # STRIP: Text, Lyrics, Markers, Cue Points, and old Track Names 
            if msg.is_meta and msg.type in ('text', 'lyrics', 'marker', 'cue_marker', 'track_name', 'copyright'):
                skipped_time += msg.time
                continue
                    
            if msg.type == 'control_change' and msg.control in (7, 10, 11): 
                skipped_time += msg.time
                continue
                
            # Copy the message and apply channel mapping if necessary
            new_msg = msg.copy(channel=chan) if hasattr(msg, 'channel') and chan is not None else msg.copy()
            
            # Add any accumulated skipped time to this message, then reset the accumulator
            new_msg.time += skipped_time
            skipped_time = 0
            
            new_track.append(new_msg)

    add_bass_track_if_required_in_mem(new_mid)
    add_percussion_track_if_required_in_mem(new_mid)
    new_mid.save(output_path)
    return new_mapping


# Create midi file with the player track quantised -- 
#  this is for midiscore to process.
# Also retain the other tracks so we detect the length of the song correctly,
#  as there could be rest bars after the last player note.
#  E.g. Gymnopdie 1.
def save_midi_with_quantised_player_track(mid, output_path, player_idx, resolution, mapping):
    new_mid = mido.MidiFile(ticks_per_beat=mid.ticks_per_beat, type=mid.type)
    keep = list(mapping.keys()) # retain all tracks
    if 0 not in keep: keep.append(0) # including control track
    
    new_player_idx = 0
    track_num = 1

    for i, track in enumerate(mid.tracks):
        if i not in keep: continue

        # Find new player track index in new midi file
        if i == player_idx:
            new_player_idx = track_num
            q_track = quantize_track(track, mid.ticks_per_beat, resolution)
            new_mid.tracks.append(mido.MidiTrack([m.copy(channel=0) if hasattr(m, 'channel') else m for m in q_track]))
        else:
            new_mid.tracks.append(mido.MidiTrack([m.copy(channel=0) if hasattr(m, 'channel') else m for m in track]))

        track_num += 1

    new_mid.save(output_path)
    return new_player_idx


def get_time_signature(mid):
    """Scans the MIDI file for a time signature meta-message."""
    for track in mid.tracks:
        for msg in track:
            if msg.type == 'time_signature':
                return msg.numerator, msg.denominator
    return None, None


def update_songs_database(composer, piece, target_dir, audio_path, num, den, score_path_string):
    """Appends the new columns to songs.csv."""
    level = "1"
    new_round = 1
    db_path = Path("../Assets/Songs") / "songs.csv" 
    
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
    
    # --- Count-in Columns ---
    # e.g., Count-in/count-in-4-4.mid
    count_in_midi = f"Count-in/count-in-{num}-{den}.mid"
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
    # 12. Life decrease
    life_decrease = "1"
    # 13. Zero-based bar numbers at the ends of sections, or -1
    sections = "-1"
 
    new_row = [
        level, str(new_round), internal_name, loc_title, loc_subtitle, loc_composer,
        midi_file_path, count_in_midi, count_in_beats, count_in_gui,
        palette, score_file, life_decrease, sections
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

def find_bpm_in_midi(mid):
    extracted_bpm = None
    # Look for tempo setting event.
    for track in mid.tracks:
        for msg in track:
            if msg.type == 'set_tempo':
                # mido.tempo2bpm returns a float, so we round it to a clean integer
                extracted_bpm = round(mido.tempo2bpm(msg.tempo))
                break 
        if extracted_bpm:
            break
    return extracted_bpm


def get_or_prompt_bpm(mid):
    """Scans for a tempo meta-message, converts to BPM, and asks user to confirm."""
    extracted_bpm = find_bpm_in_midi(mid)
    
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


def write_channels_txt(target_dir, mapping):
    # Create a reverse map for naming: {0: 'Player', 7: 'Bass', ...}
    # We capitalize the keys (Piano-C, etc.)
    chan_to_name = {v: k.replace('-', ' ').title().replace(' ', '-') for k, v in ROLE_CHANNELS.items()}
    
    with open(target_dir / "channels.txt", 'w') as f:
        for k in mapping:
            f.write(f"{k}: {mapping[k]} # Track {k} -> {chan_to_name[mapping[k]]}\n")


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
    analyze_tracks(mid) # Just to show track list
    # Extract top line from a polyphonic track if required, adding an extra track
    extract_top_line_in_mem(mid)
    # Map tracks to channels: first, show new track list
    track_data = analyze_tracks(mid)
    # Get track -> channel map
    mapping, player_idx = get_user_mapping(track_data)
    # Grab the specific player track and pass it to the setting prompt
    player_track = mid.tracks[player_idx]
    num_res, q_flag = get_quantization_setting(player_track, mid.ticks_per_beat)

    # 3. Time Signature and Tempo
    num, den = get_or_prompt_time_signature(mid)
    orig_bpm = find_bpm_in_midi(mid)
    new_bpm = get_or_prompt_bpm(mid)
    if (orig_bpm != new_bpm):
        set_midi_bpm_in_mem(mid, float(new_bpm))
 
    # 4. Metadata Gathering
    piece_name = input("\n📝 Enter the name of the Piece: ")
    composer_name = input("📝 Enter the name of the Composer: ")

    comp_camel = to_camel_case(composer_name)
    piece_camel = to_camel_case(piece_name)

    # 5. File System Setup
    target_dir, audio_path, score_midi_path, workspace_dir = setup_paths(composer_name, piece_name)
    print(f"\n📁 Created directory: {target_dir}")
    shutil.copy(input_path, workspace_dir / "original_backup.mid")

    # 6. MIDI Pipeline Processing
    print(f"⚙️ Generating Audio File: {audio_path.name}...")
    # returns new track->channel mapping
    new_mapping = process_audio_pipeline(mid, audio_path, mapping)
    
    print(f"🎼 Generating Score File: {score_midi_path.name}...")
    new_player_idx = save_midi_with_quantised_player_track(mid, score_midi_path, player_idx, num_res, mapping)
    
    # 7. Execute External Score Binaries (midiscore & makescore)
    score_csv_string = generate_score_files(
        comp_camel, piece_camel, target_dir, workspace_dir, score_midi_path, num, den, q_flag, new_bpm, new_player_idx
    )
    
    # 8. Update the Game Database
    if score_csv_string:
        update_songs_database(
            composer_name, piece_name, target_dir, audio_path, num, den, score_csv_string
        )
    else:
        print("\n⚠️ Database update skipped due to score generation failure.")
        return
    
    # 9. Write channels.txt in case fix_channels.py is required
    write_channels_txt(workspace_dir, new_mapping)

    print(f"\n🎉 Finished! Song successfully added to the game pipeline.")

if __name__ == "__main__":
    main()


