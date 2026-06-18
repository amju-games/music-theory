# add_percussion_track.py
# Adds an auto-generated, step-sequenced percussion track to a MIDI file.
# Expressions are handled via tokenized strings and velocity humanization.

import sys
import mido
import random
from pathlib import Path

# --- EXTENDED GENERAL MIDI DRUM MAP ---
DRUM_MAP = {
    'kick': 36, 'snare': 38, 'side_stick': 37,
    'closed_hihat': 42, 'pedal_hihat': 44, 'open_hihat': 46,
    'ride': 51, 'ride_bell': 53,
    'hi_bongo': 60, 'low_bongo': 61,
    'mute_hi_conga': 62, 'open_hi_conga': 63, 'low_conga': 64,
    'short_guiro': 73, 'long_guiro': 74,
    'mute_triangle': 80, 'open_triangle': 81,
    'shaker': 82
}
PERC_CHANNEL = 9  # MIDI Channel 10 is reserved for percussion

# --- THE DATA-DRIVEN STYLE DATABASE ---
# 'x' = Normal Hit, 'o' = Accented Hit, '.' = Rest
# Menu updates, parsing, and math scale automatically based on this data.
STYLE_DATABASE = {
    1: {
        "name": "Standard (Pop/Rock - 16 Step Grid)",
        "steps_per_bar": 16,
        "patterns": {
            1: {
                'kick':         "x.......x.......",
                'snare':        "....x.......x...",
                'closed_hihat': "x.x.x.x.x.x.x.x."
            },
            2: {
                'kick':         "x...x...x.......",
                'snare':        "....x.......x..x",
                'closed_hihat': "x.x.x.x.x.x.x.x."
            },
            3: {
                'kick':         "x...x...x.x.....",
                'snare':        "....x..x....x..x",
                'closed_hihat': "xxxxxxxxxxxxxxxx"
            }
        }
    },
    2: {
        "name": "Jazz / Swing (Triplet - 12 Step Grid)",
        "steps_per_bar": 12,  # 4 beats * 3 triplet subdivisions per beat
        "patterns": {
            1: {
                'ride':         "x.xx.xx.xx.x",  # Classic "1 - and-a 2 - and-a" swing feel
                'pedal_hihat':  "...x.....x.."   # Foot chick on 2 and 4
            },
            2: {
                'ride':         "x.xx.xx.xx.x",
                'pedal_hihat':  "...x.....x..",
                'kick':         "x...........",  # Light, non-intrusive feathered kick on 1
                'snare':        "......x....."   # Single soft syncopated comping hit
            },
            3: {
                'ride':         "x.ox.ox.ox.o",  # Accented ride skips
                'pedal_hihat':  "...x.....x..",
                'kick':         "x.....x.....",
                'snare':        "....x.....x."   # Busy comping dialogue
            }
        }
    },
    3: {
        "name": "Latin / Hand Percussion (16 Step Grid)",
        "steps_per_bar": 16,
        "patterns": {
            1: {
                'shaker':       "x.x.x.x.x.x.x.x.",
                'low_conga':    "x.......x.......",
                'open_hi_conga':"....x.......x..."
            },
            2: {
                'shaker':       "x.x.x.x.x.x.x.x.",
                'low_conga':    "x...x...x.......",
                'open_hi_conga':"....x.......x...",
                'mute_hi_conga':".......x.......x"
            },
            3: {
                'shaker':       "xxxxxxxxxxxxxxxx",
                'low_conga':    "x...x...x.x.....",
                'open_hi_conga':"....x.......x...",
                'mute_hi_conga':".......x.......x",
                'long_guiro':   "..x...x...x...x."
            }
        }
    },
    4: {
        "name": "Orchestral Light (16 Step Grid)",
        "steps_per_bar": 16,
        "patterns": {
            1: { 'open_triangle': "x..............." },
            2: { 'open_triangle': "x...............", 'ride': "........x......." },
            3: { 'open_triangle': "x.......x.......", 'ride': "....x.......x..." }
        }
    },
    5: {
        "name": "Bossa Nova (Syncopated Smooth - 16 Step Grid)",
        "steps_per_bar": 16,
        "patterns": {
            1: {
                'shaker':       "x.x.x.x.x.x.x.x.",  # Steady timekeeping swing
                'low_conga':    "x.......x.......",  # Grounding downbeats
                'side_stick':   "x..x..x...x..x.."   # Classic syncopated Bossa rim pattern
            },
            2: {
                'shaker':       "x.x.x.x.x.x.x.x.",
                'low_conga':    "x...x...x...x...",  # Mimics the steady acoustic bass drum sway
                'side_stick':   "o..x..o...x..x..",  # Accented points on the rim pattern
                'open_triangle':"............x..."   # Subtle high bell texture
            },
            3: {
                'shaker':       "xxxxxxxxxxxxxxxx",  # High velocity push
                'low_conga':    "x..x..x.x..x..x.",  # Syncopated low-end movement
                'side_stick':   "o..x..o...o..x.x",  # Extended rhythmic cadence
                'open_triangle':"....x.......x..."
            }
        }
    },
    # -------------------------------------------------------------
    # STYLE 6: JAZZ WALTZ (3/4 Time - 12 Step Grid)
    # -------------------------------------------------------------
    6: {
        "name": "Jazz Waltz (3/4 Time - 12 Step Grid)",
        "steps_per_bar": 12,  # 3 beats * 4 subdivisions per beat
        "patterns": {
            1: {
                'ride':         "x..xx..xx..x",  # Swinging quarter + two eighths feel
                'pedal_hihat':  "....x......."   # Foot chick strictly on beat 2
            },
            2: {
                'ride':         "x..xx.ox..xx",  # Accented push into beat 3
                'pedal_hihat':  "....x.......",
                'kick':         "x...........",  # Grounding beat 1
                'snare':        "........x..."   # Soft conversation on the back end
            },
            3: {
                'ride':         "x.ox.ox.ox.o",  # Intense constant triplet swing drive
                'pedal_hihat':  "....x...x...",  # Foot chick on 2 and 3
                'kick':         "x...........",
                'snare':        "....x..x.x.."   # Syncopated comping between the beats
            }
        }
    },
    # -------------------------------------------------------------
    # STYLE 7: SIX-FOUR GROOVE (6/4 Time - 24 Step Grid)
    # -------------------------------------------------------------
    7: {
        "name": "Six-Four Afro-Cuban/Jazz (6/4 Time - 24 Step Grid)",
        "steps_per_bar": 24,  # 6 beats * 4 subdivisions per beat
        "patterns": {
            1: {
                'shaker':       "x.x.x.x.x.x.x.x.x.x.x.x.", # Smooth 8th note stream
                'low_conga':    "x...........x...........", # Anchors downbeats 1 and 4
                'side_stick':   "....x..x........x..x...." # Classic dotted-quarter feel
            },
            2: {
                'shaker':       "x.x.x.x.x.x.x.x.x.x.x.x.",
                'low_conga':    "x.....x.....x.....x.....", # Dotted half-note sway
                'side_stick':   "....x..x..x.....x..x..o.", # Expanded syncopation with an accent
                'open_triangle':"............x..........." 
            },
            3: {
                'shaker':       "xxxxxxxxxxxxxxxxxxxxxxxx", # Driving 16ths
                'low_conga':    "x..x..x..x..x..x..x..x..", # Intense rolling cross-rhythm
                'side_stick':   "o...x..o...x..o...x..o.x", # Full-bar Afro-Cuban bembe adaptation
                'open_triangle':"......x...........x....."
            }
        }
    }
}

# --- HELPER LOGIC ---
def get_total_ticks(mid):
    max_ticks = 0
    for track in mid.tracks:
        ticks = sum(msg.time for msg in track)
        if ticks > max_ticks: max_ticks = ticks
    return max_ticks

def humanize(base_vel, variance=12, is_kick=False):
    """Adds natural human variation to velocity. Kicks remain steady."""
    if is_kick:
        return base_vel
    return max(1, min(127, base_vel + random.randint(-variance, variance)))

# --- STEP-PARSER MATHEMATICAL ENGINE ---
def generate_drum_pattern(num, den, style_choice, intensity, ticks_per_beat, total_ticks, anacrusis_ticks):
    events = []
    
    style_config = STYLE_DATABASE.get(style_choice, STYLE_DATABASE[1])
    steps_per_bar = style_config["steps_per_bar"]
    active_patterns = style_config["patterns"].get(intensity, style_config["patterns"][2])
    
    beat_length = int(ticks_per_beat * (4 / den))
    bar_length = beat_length * num
    tick_per_step = bar_length / steps_per_bar
    
    current_bar_tick = anacrusis_ticks
    
    while current_bar_tick < total_ticks:
        for inst_name, pattern_string in active_patterns.items():
            note_num = DRUM_MAP[inst_name]
            is_kick = (inst_name in ('kick', 'low_conga'))  # Anchor notes bypass humanization
            
            for step_idx, char in enumerate(pattern_string):
                if step_idx >= steps_per_bar: 
                    break
                    
                if char in ('x', 'o'):
                    step_tick = int(current_bar_tick + (step_idx * tick_per_step))
                    
                    # 'o' triggers accented hits, 'x' triggers standard hits
                    base_vel = 105 if char == 'o' else 75
                    vel = humanize(base_vel, is_kick=is_kick)
                    
                    events.append((step_tick, note_num, vel))
                    
        current_bar_tick += bar_length

    # Boundary safety clipping
    events = [e for e in events if e[0] <= total_ticks]
    
    final_events = []
    for abs_time, note, vel in events:
        final_events.append((abs_time, 'note_on', note, vel))
        final_events.append((abs_time + 40, 'note_off', note, 0)) # Fixed duration note offs
        
    final_events.sort(key=lambda x: x[0])
    return final_events

# --- AUTO-GENERATING USER INTERFACE ---
def ask_user_for_style():
    print("\n🎼 Select Style:")
    for style_id, config in STYLE_DATABASE.items():
        print(f"   {style_id}: {config['name']}")
        
    while True:
        try:
            choice = input(f"   Choice [1-{len(STYLE_DATABASE)}]: ").strip()
            if not choice: return 1
            style_id = int(choice)
            if style_id in STYLE_DATABASE:
                return style_id
            print(f"❌ Selection out of bounds. Please choose 1 to {len(STYLE_DATABASE)}.")
        except ValueError:
            print("❌ Invalid input. Please type a number.")

def ask_if_percussion_is_required():
    yes_perc = input("🍔 Would you like me to add auto-generated percussion? (y/n): ").strip().lower()
    return yes_perc == 'y'

def add_percussion_track_if_required_in_mem(mid):
    # Add a drum track to existing midi object in mem
    if not ask_if_percussion_is_required():
        return mid
    add_percussion_track_to_mid(mid)

def add_percussion_track():
    # Create a new midi file with drum track added
    print("\n🥁 Data-Driven Drum Engine v2.0")
    print("-" * 40)
    
    if len(sys.argv) != 2:
        print("Usage: python3 add_percussion_track.py input.mid")
        sys.exit(1)

    if not ask_if_percussion_is_required():
        return

    input_filename = sys.argv[1]
    try:
        mid = mido.MidiFile(input_filename)
    except Exception as e:
        sys.exit(f"❌ Error loading MIDI: {e}")

    add_percussion_track_to_mid(mid)

    # Save file
    output_filename = f"{Path(input_filename).stem}_with_perc.mid"
    mid.save(output_filename)
    print(f"   💾 Saved new midi file to: {output_filename}\n")


def ask_user_anacrusis():
    while True:
        ana = input("⏱️  Is there an anacrusis? (q/q./c/c./m/m.) or Enter if none: ").strip()
        if not ana: 
            return 0

        ana_lookup = {
            "q" :  0.5,
            "q." : 0.75,
            "c" :  1.0,
            "c." : 1.5,
            "m" :  2.0,
            "m." : 2.5
        }
        if ana in ana_lookup:
            tpq_mult = ana_lookup[ana]
            return tpq_mult
 
        print("❌ Sorry, I didn't recognise that. Please use one of q/q./c/c./m/m.") 


def add_percussion_track_to_mid(mid):
    # Gather settings
    ts_input = input("⏱️  Time signature (e.g., 4/4): ").strip()
    try:
        num, den = map(int, ts_input.split('/'))
    except ValueError:
        print("⚠️  Invalid format. Defaulting to 4/4.")
        num, den = 4, 4

    anacrusis_ticks = ask_user_anacrusis() * mid.ticks_per_beat

    style_choice = ask_user_for_style()

    try:
        intensity = int(input("\n🔥 Intensity/Busyness (1=Sparse, 3=Busy) [2]: ").strip() or 2)
        intensity = max(1, min(3, intensity))
    except ValueError:
        intensity = 2

    # Sequencer calculations
    total_ticks = get_total_ticks(mid)
    abs_events = generate_drum_pattern(num, den, style_choice, intensity, mid.ticks_per_beat, total_ticks, anacrusis_ticks)

    # Convert sorted global triggers to structural standard relative delta track
    perc_track = mido.MidiTrack()
    cleaned_style_name = STYLE_DATABASE[style_choice]["name"].split(" (")[0]
    perc_track.append(mido.MetaMessage('track_name', name=f"Percussion ({cleaned_style_name})", time=0))
    
    last_time = 0
    for abs_time, msg_type, note, vel in abs_events:
        delta = abs_time - last_time
        perc_track.append(mido.Message(msg_type, note=note, velocity=vel, time=delta, channel=PERC_CHANNEL))
        last_time = abs_time

    mid.tracks.append(perc_track)
    print(f"\n   ✅ Success! Processed {len(abs_events)//2} humanized drum steps.") 

if __name__ == "__main__":
    add_percussion_track()

