#include "Pitch.h"

namespace MidiScore
{
std::string PitchToString(int pitch)
{
  return std::to_string(pitch);
}

std::string PitchToString(int pitch, KeySig ks) 
{
    // Standard MIDI range check
    if (pitch < 0 || pitch > 127) return "Invalid";

    // MIDI pitch 60 is C4. The octave increments every 12 notes.
    int octave = (pitch / 12) - 1;
    int note_class = pitch % 12;

    // Determine if the key signature dictates flats.
    // KS_FLAT_0 (C Major/A Minor) and all sharp keys default to sharp spelling.
    bool use_flats = (ks >= KeySig::KS_FLAT_0 && ks <= KeySig::KS_FLAT_7);

    // Parallel arrays to assemble the note name and accidental
    const char sharp_letters[] = {'c', 'c', 'd', 'd', 'e', 'f', 'f', 'g', 'g', 'a', 'a', 'b'};
    const char sharp_accids[]  = {'\0', '+', '\0', '+', '\0', '\0', '+', '\0', '+', '\0', '+', '\0'};

    const char flat_letters[] = {'c', 'd', 'd', 'e', 'e', 'f', 'g', 'g', 'a', 'a', 'b', 'b'};
    const char flat_accids[]  = {'\0', '-', '\0', '-', '\0', '\0', '-', '\0', '-', '\0', '-', '\0'};

    std::string result;
    
    if (use_flats) {
        result += flat_letters[note_class];
        result += std::to_string(octave);
        if (flat_accids[note_class] != '\0') result += flat_accids[note_class];
    } else {
        result += sharp_letters[note_class];
        result += std::to_string(octave);
        if (sharp_accids[note_class] != '\0') result += sharp_accids[note_class];
    }

    return result;
}
}

