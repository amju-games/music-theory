# music_utils.py
import re

def note_to_midi(note_input) -> int:
    """
    Converts a note string (e.g., 'C4', 'Bb3', 'A#3') or raw MIDI integer string 
    into a valid MIDI note number (0-127). Standard Middle C (C4) = 60.
    """
    if not note_input:
        raise ValueError("Input cannot be empty.")
        
    note_str = str(note_input).strip().lower()

    # If the user just typed a raw MIDI number, return it immediately
    if note_str.isdigit():
        return int(note_str)

    # Regex to pull out: [step] [optional accidentals] [octave]
    # Example: "bb3" -> group(1)='b', group(2)='b', group(3)='3'
    match = re.match(r"^([a-g])([#b♯♭]*)(-?\d+)$", note_str)
    if not match:
        raise ValueError(f"Invalid note format: '{note_input}'. Use formats like C4, Bb3, or 48.")

    step, alter, octave_str = match.groups()
    octave = int(octave_str)

    # Base pitch classes within an octave
    pitch_classes = {
        'c': 0, 'd': 2, 'e': 4, 'f': 5, 'g': 7, 'a': 9, 'b': 11
    }
    
    midi_note = pitch_classes[step]

    # Handle accidental modifications (can stack them, though rare)
    for char in alter:
        if char in ('#', '♯'):
            midi_note += 1
        elif char in ('b', '♭'):
            midi_note -= 1

    # Standard MIDI calculation: C4 is note 60 (Octave 4)
    midi_note += (octave + 1) * 12

    # Restrict to valid MIDI boundaries
    return max(0, min(127, midi_note))


