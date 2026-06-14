# extract_top_line_in_mem.py
import sys
import mido
from pathlib import Path
from collections import defaultdict

# --- 1. GENERAL HELPERS ---

def get_track_name(track, default_index):
    """Finds the metadata name of a track, or returns a generic default."""
    for msg in track:
        if msg.type == 'track_name':
            return msg.name
    return f"Track {default_index}"

def has_playable_notes(track):
    """Checks if a track contains actual performance data."""
    return any(msg.type == 'note_on' and msg.velocity > 0 for msg in track)

# --- 2. PIPELINE PROCESSING FUNCTIONS ---

def parse_track_to_notes(track):
    """STEP 1: Parses raw MIDI track messages into absolute-time note objects."""
    abs_time = 0
    pending = defaultdict(list)
    raw_notes = []
    non_note_events = []

    for msg in track:
        abs_time += msg.time
        if msg.type == 'note_on' and msg.velocity > 0:
            pending[msg.note].append((abs_time, msg.velocity, msg.channel))
        elif msg.type in ('note_off', 'note_on'):
            if pending[msg.note]:
                start_t, vel, ch = pending[msg.note].pop(0)
                raw_notes.append({
                    'pitch': msg.note, 'start': start_t, 'end': abs_time, 
                    'velocity': vel, 'channel': ch
                })
        else:
            non_note_events.append((abs_time, msg.copy()))

    for pitch, note_instances in pending.items():
        for start_t, vel, ch in note_instances:
            raw_notes.append({
                'pitch': pitch, 'start': start_t, 'end': abs_time, 'velocity': vel, 'channel': ch
            })

    return raw_notes, non_note_events


def cluster_notes_into_chords(raw_notes, tolerance):
    """STEP 2: Groups notes into chord clusters using a rolling time window."""
    if not raw_notes:
        return []

    raw_notes.sort(key=lambda x: x['start'])
    chords = []
    current_chord = [raw_notes[0]]

    for note in raw_notes[1:]:
        if note['start'] - current_chord[0]['start'] <= tolerance:
            current_chord.append(note)
        else:
            chords.append(current_chord)
            current_chord = [note]
            
    chords.append(current_chord)
    return chords


def should_reject_melody_candidate(candidate, last_top_note, max_drop):
    """Evaluates a candidate note against running monophonic and interval constraints."""
    if last_top_note is None:
        return False
    if candidate['start'] < last_top_note['end']:
        return True
    return (last_top_note['pitch'] - candidate['pitch']) > max_drop


def split_chords_into_lines(chords, max_drop):
    """STEP 3: Standard filtering pass to separate chords into lead vs accompaniment."""
    top_notes = []
    bottom_notes = []
    last_top_note = None

    for chord in chords:
        chord.sort(key=lambda x: x['pitch'], reverse=True)
        candidate = chord[0]

        if should_reject_melody_candidate(candidate, last_top_note, max_drop):
            bottom_notes.extend(chord)
        else:
            top_notes.append(candidate)
            last_top_note = candidate
            bottom_notes.extend(chord[1:])

    return top_notes, bottom_notes


def prune_intro_notes(top_notes, max_drop):
    """
    STEP 4 (SECOND PASS): Detects if early top notes are actually an accompaniment 
    intro by identifying massive upward leaps into the true melody register.
    """
    if len(top_notes) < 2:
        return top_notes, []

    # Look at the first ~15 notes to check the song setup phase
    for i in range(1, min(len(top_notes), 15)):
        pitch_jump = top_notes[i]['pitch'] - top_notes[i-1]['pitch']
        
        # If there's a huge upward jump out of the initial pattern
        if pitch_jump > max_drop:
            intro_candidate = top_notes[:i]
            melody_candidate = top_notes[i:]
            
            avg_intro_pitch = sum(n['pitch'] for n in intro_candidate) / len(intro_candidate)
            avg_mel_pitch = sum(n['pitch'] for n in melody_candidate) / len(melody_candidate)
            
            # Sanity Check: Only prune if the intro is distinctly lower than the melody register
            if avg_mel_pitch - avg_intro_pitch > 3:
                return melody_candidate, intro_candidate
                
    return top_notes, []

# --- 3. RECONSTRUCTION AND INTERACTION ---

def build_track_events(notes, extra_events, track_name=None):
    """Converts absolute-timed notes and extra events into relative-delta MIDI messages."""
    events = []
    if track_name:
        events.append((0, mido.MetaMessage('track_name', name=track_name, time=0)))
    for abs_t, msg in extra_events:
        events.append((abs_t, msg))
    for note in notes:
        events.append((note['start'], mido.Message('note_on', note=note['pitch'], velocity=note['velocity'], channel=note['channel'])))
        events.append((note['end'],   mido.Message('note_off', note=note['pitch'], velocity=0, channel=note['channel'])))
    events.sort(key=lambda x: x[0])
    return events


def apply_events_to_track(track, absolute_events):
    """Clears a track and writes absolute events into it using relative delta times."""
    track.clear()
    last_time = 0
    for abs_t, msg in absolute_events:
        msg.time = abs_t - last_time
        track.append(msg)
        last_time = abs_t


def get_source_track_and_cutoff(mid):
    """Prompts the user to select a source track and custom pitch drop cutoff."""
    print("\n🎹 Scanning available tracks for Top Line extraction...")
    valid_tracks = []
    for i, track in enumerate(mid.tracks):
        if not has_playable_notes(track) or getattr(track, 'channel', -1) == 9: 
            continue
        valid_tracks.append(track)
        
    if not valid_tracks:
        print("   ⚠️ No valid melodic tracks found.")
        return None, 9

    for i, track in enumerate(valid_tracks):
        print(f"   [{i + 1}] {get_track_name(track, i)}")
        
    while True:
        ans = input(f"   Select track to extract from [1-{len(valid_tracks)}] or Enter to skip: ").strip()
        if not ans: return None, 9
        try:
            choice = int(ans) - 1
            if 0 <= choice < len(valid_tracks):
                selected_track = valid_tracks[choice]
                break
        except ValueError: pass
        print("   ❌ Invalid choice. Please try again.")

    cutoff_ans = input("   Enter max downward melodic jump in semitones [9 = Major 6th]: ").strip()
    max_drop = int(cutoff_ans) if cutoff_ans.isdigit() else 9
    return selected_track, max_drop


def extract_top_line_in_mem(mid):
    """Main pipeline: Runs the multi-pass extraction process entirely in RAM."""
    source_track, max_drop = get_source_track_and_cutoff(mid)
    if not source_track:
        print("  ⚠️  Skipping Top Line extraction.")
        return mid

    original_name = get_track_name(source_track, "Source Track")
    tolerance = max(1, mid.ticks_per_beat // 16)
    
    # Pass 1: Parse and initial chord separation
    raw_notes, extra_events = parse_track_to_notes(source_track)
    chords = cluster_notes_into_chords(raw_notes, tolerance)
    top_notes, bottom_notes = split_chords_into_lines(chords, max_drop)
    
    # Pass 2: Clean out accompaniment intro layers
    top_notes, pruned_intro = prune_intro_notes(top_notes, max_drop)
    bottom_notes.extend(pruned_intro) # Safely return intro notes to the accompaniment pool

    if not top_notes:
        print("  ⚠️  No notes met the melody criteria after intro pruning.")
        return mid

    # Mutate original track (Keep accompaniment + controllers)
    bottom_events = build_track_events(bottom_notes, extra_events)
    apply_events_to_track(source_track, bottom_events)

    # Build and append the new TopLine track
    top_events = build_track_events(top_notes, [], track_name=f"{original_name} (TopLine)")
    top_track = mido.MidiTrack()
    apply_events_to_track(top_track, top_events)
    mid.tracks.append(top_track)

    print(f"\n  ✂  Smart Top Line Extraction Report for '{original_name}':")
    print(f"     • Total Notes Processed:        {len(top_notes) + len(bottom_notes)}")
    print(f"     • Extracted Lead Melody:       ✨ {len(top_notes)} ✨")
    print(f"     • Pruned Intro Notes Demoted:   {len(pruned_intro)}")
    print(f"     • Left in Accompaniment:        {len(bottom_notes) - len(pruned_intro)}")
    
    return mid

# --- 4. STANDALONE ISOLATION TESTING SUITE ---

def extract_top_line(input_filename):
    try:
        mid = mido.MidiFile(input_filename)
    except Exception as e:
        print(f"Error loading MIDI file: {e}")
        sys.exit(1)
        
    extract_top_line_in_mem(mid)
    
    output_filename = f"{Path(input_filename).stem}_split_topline.mid"
    mid.save(output_filename)
    print(f"   💾 Saved new midi file to: {output_filename}\n")
             
     
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 extract_top_line.py input.mid")
        sys.exit(1) 
    extract_top_line(sys.argv[1])

